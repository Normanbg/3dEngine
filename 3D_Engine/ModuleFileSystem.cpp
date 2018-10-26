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
		MODELS_PATH, TEXTURES_PATH, AUDIO_PATH, LIBRARY_PATH, SETTINGS_PATH
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

void ModuleFileSystem::writeFile(const char * fileName, const void * data, uint bytes)
{


}

uint ModuleFileSystem::readFile(const char * fileName, void ** data, uint bytes)
{
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
				delete[] data;
				return;
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
