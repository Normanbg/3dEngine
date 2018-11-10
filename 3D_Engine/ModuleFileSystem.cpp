#include "Application.h"
#include "ModuleFileSystem.h"
#include "PhysFS/include/physfs.h"


#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

ModuleFileSystem::ModuleFileSystem(bool start_enabled) : Module(start_enabled)
{
	name = "fileSystem";

	// needs to be created before Init so other modules can use it
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	if (PHYSFS_setWriteDir(".") == 0)
		OWN_LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	addPath(".");

	//Create main files if they do not exist and add them to the search path
	const char* mainPaths[] = {
		MODELS_PATH, TEXTURES_PATH, AUDIO_PATH, LIB_MODELS_PATH, LIB_TEXTURES_PATH, SETTINGS_PATH, SCENES_PATH
	};
	for (uint i = 0; i < NUM_PATHS; ++i)
	{
		if (PHYSFS_exists(mainPaths[i]) == 0)
			PHYSFS_mkdir(mainPaths[i]);

		addPath(mainPaths[i]);
	}
}


ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::addPath(const char * path)
{	bool ret = true;
	 
	if (PHYSFS_mount(path, nullptr, 1) == 0) {
		OWN_LOG("File System error while adding a path: %s\n", PHYSFS_getLastError());
		ret = false;
	}
	return ret;
}

uint ModuleFileSystem::writeFile(const char * fileName, const void * data, uint bytes	)
{
	PHYSFS_file* file = PHYSFS_openWrite(fileName);
	if (file == nullptr)
	{
		OWN_LOG("Error opening file. Error:", PHYSFS_getLastError());
		return 0;
	}
	else{	

		uint written = PHYSFS_write(file, (const void*)data, 1, bytes);
		PHYSFS_close(file);

		if (written != bytes)
		{
			OWN_LOG("Writting file error. Not all data readen Error:  %s", PHYSFS_getLastError());
			return 0;
		}

		return written;
	}
	

}

uint ModuleFileSystem::readFile(const char * fileName, char** data)
{
	NormalizePath((char*)fileName);
	PHYSFS_file* file = PHYSFS_openRead(fileName);
	uint ret = 0;

	if (file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(file);

		if (size > 0)
		{
			*data = new char[size];
			uint readed = (uint)PHYSFS_read(file, *data, 1, size);
			
			if (readed != size)
			{
				OWN_LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				
				return ret;
			}
			else
				
				ret = readed;
		}
		PHYSFS_close(file);
	}
	else
		OWN_LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

void ModuleFileSystem::CopyFileTo(const char * dest, const char * origin)
{
	char *data;
	uint size = readFile(origin, &data);
	writeFile(origin, data, size);
}

void ModuleFileSystem::CopyDDStoLib(const char * path) 
{
	std::string ddsName;
	GetNameFromPath(path, nullptr, &ddsName, nullptr, nullptr);
	std::string libpath = LIB_TEXTURES_PATH + ddsName + DDS_FORMAT;
	CopyFileTo(libpath.c_str(), path);
}

void ModuleFileSystem::GetNameFromPath(const char * full_path, std::string * path, std::string * file, std::string * fileWithExtension, std::string * extension) const
{
	if (full_path != nullptr) 
	{
		std::string nwFullPath = full_path;
		NormalizePath(nwFullPath);
		uint posSlash = nwFullPath.find_last_of("\\/");
		uint posDot = nwFullPath.find_last_of(".");

		if (path != nullptr)
		{
			if (posSlash < nwFullPath.length())
				*path = nwFullPath.substr(0, posSlash + 1);
			else
				path->clear();
		}
		if (fileWithExtension != nullptr) {
			if (posSlash < nwFullPath.length()) {
				*fileWithExtension = nwFullPath.substr(posSlash + 1);
			}
			else
				*fileWithExtension = nwFullPath;
		}

		if (file != nullptr)
		{	
			nwFullPath = nwFullPath.substr(0, posDot);
			posSlash = nwFullPath.find_last_of("\\/");
			*file = nwFullPath.substr(posSlash + 1);
			
		}		

		if (extension != nullptr)
		{
			if (posDot < nwFullPath.length())
				*extension = nwFullPath.substr(posDot );
			else
				extension->clear();
		}
	}

}
void ModuleFileSystem::NormalizePath(char * full_path, bool toLower) const
{
	uint len = strlen(full_path);
	for (int i = 0; i < len; ++i)
	{
		if (full_path[i] == '\\')
			full_path[i] = '/';
		else
			if (toLower) {
				full_path[i] = tolower(full_path[i]);
			}
	}
}
void ModuleFileSystem::NormalizePath(std::string & full_path, bool toLower) const
{
	for (std::string::iterator charIterator = full_path.begin(); charIterator != full_path.end(); ++charIterator)
	{
		if (*charIterator == '\\')
			*charIterator = '/';
		else
			if (toLower) {
				*charIterator = tolower(*charIterator);
			}
	}
}

bool ModuleFileSystem::Copy(const char * source, const char * destination)
{
	bool ret = false;
	char buf[8192];

	PHYSFS_file* src = PHYSFS_openRead(source);
	PHYSFS_file* dst = PHYSFS_openWrite(destination);

	PHYSFS_sint32 size;
	if (src && dst)
	{
		while (size = (PHYSFS_sint32)PHYSFS_read(src, buf, 1, 8192))
			PHYSFS_write(dst, buf, 1, size);

		PHYSFS_close(src);
		PHYSFS_close(dst);
		ret = true;

		OWN_LOG("File System copied file [%s] to [%s]", source, destination);
	}
	else
		OWN_LOG("File System error while copy from [%s] to [%s]", source, destination);

	return ret;
}

void ModuleFileSystem::DiscoverFiles(const char* directory, std::vector<std::string> & fileList, std::vector<std::string> & directoryList) const
{
	char **totalFiles = PHYSFS_enumerateFiles(directory);
	char **i;

	std::string dir(directory);

	for (i = totalFiles; *i != nullptr; i++)
	{
		if (PHYSFS_isDirectory((dir + *i).c_str()))
			directoryList.push_back(*i);
		else
			fileList.push_back(*i);
	}

	PHYSFS_freeList(totalFiles);
}
