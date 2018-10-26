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

	void writeFile(const char* fileName, const void* data, uint bytes);
	uint readFile(const char* fileName, void** data, uint bytes);
};

#endif // __MODULE_FS_H__