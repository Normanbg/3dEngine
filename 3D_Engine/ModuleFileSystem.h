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



	void CopyFileTo(const char* dest, const char* origin);
	void CopyDDStoLib(const char* path);

	void NormalizePath(char * full_path, bool toLower = false) const;

	void NormalizePath(std::string & full_path, bool toLower = false) const;

	
	void GetNameFromMesh(const char* meshName, std::string * name, uint& num );
	void GetNameFromPath(const char * full_path, std::string * path, std::string * file, std::string * fileWithExtension, std::string * extension) const;
};

#endif // __MODULE_FS_H__