#ifndef __MODULE_FS_H__
#define __MODULE_FS_H__

#include "Module.h"
#include "Globals.h"

class ModuleFileSystem :
	public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	bool addPath(const char* path);

	uint writeFile(const char* fileName, const void* data, uint bytes);
	uint readFile(const char* fileName, char** data);

	void NormalizePath(char * full_path) const;

	void NormalizePath(std::string & full_path) const;

	void GetNameFromPath(const char * full_path, std::string * path, std::string * file, std::string * extension) const;
};

#endif // __MODULE_FS_H__