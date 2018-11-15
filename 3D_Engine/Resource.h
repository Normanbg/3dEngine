#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Application.h"
#include "Globals.h"
#include <string>

class Config;

class Resource
{
public:
	enum ResType {
		Texture,
		Mesh,
		Scene,
		Audio,
		None
	};

public:
	Resource(uuid UUID, ResType type );
	virtual	~Resource();

	inline ResType GetType() const { return type; }
	inline uuid GetUUID() const { return UUID; }
	inline const char* GetName() const { return fileName.c_str(); }
	inline std::string GetNameStr() const { return fileName; }
	inline std::string GetExportedFiles() const { return exportedFile; }
	inline const char* GetPath() const { return path.c_str(); }
	inline std::string GetPathStr() const { return path; }

	inline void SetName(const char* file) { fileName = file; }	
	inline void SetExportedFile(std::string files) { exportedFile = files; }
	inline void SetPath(const char* _path) { path = _path; }
		
	uint GetCountReferences() const { return references; }
	bool IsLoaded() const { return loaded; }

	virtual void Save(Config& config) const {};
	virtual void Load(const Config& config) {};
	virtual void LoadInMemory() {};
	virtual void FreeInMemory() {};
	virtual void CleanUp() {};

protected:
	ResType type = None;
	uuid UUID = 0;
	std::string fileName;
	std::string path;
	std::string exportedFile;
	uint references = 0;
	bool loaded = false;

};

#endif