#ifndef __MODULE_FS_H__
#define __MODULE_FS_H__

#include "Module.h"
#include "Globals.h"


class ModuleFileSystem :public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	bool addPath(const char* path);

	uint writeFile(const char* fileName, const void* data, uint bytes);
	uint readFile(const char* fileName, char** data);


	
	
	bool CopyDDStoLib(const char* path, std::vector<std::string>* written);
	bool CopyPEItoLib(const char* path, std::vector<std::string>* written = nullptr, uuid forceUUID = 0);
	bool CopyAudioToLib(const char * path, std::vector<std::string>* written = nullptr);

	void NormalizePath(char * full_path, bool toLower = false) const;

	void NormalizePath(std::string & full_path, bool toLower = false) const;

	bool Copy(const char * source, const char * destination);

	bool IsMetaFile(std::string file);
	uint GetLastModification(const char* file) const;
	void GetUUID_PEI(const char* fullName, std::string * uuid, std::string * pei);

	void GetFilesFromDir(const char * directory, std::vector<std::string>& fileList, std::vector<std::string>& directoryList, bool recursive = false) const;

	void GetNameFromPath(const char * full_path, std::string * path, std::string * file, std::string * fileWithExtension, std::string * extension) const;

};

#endif // __MODULE_FS_H__