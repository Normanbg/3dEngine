#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Globals.h"

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
	Resource(UUID uuid,ResType type );
	virtual	~Resource();

	inline const ResType GetType() const { return type; }
	inline const UUID GetUUID() const { return uuid; }
	inline const char* GetFile() const { return fileName.c_str(); }
	inline std::string GetFileStr() const { return fileName; }
	inline const char* GetExportedFile() const { return exportedFile.c_str(); }
	inline std::string GetExportedFileStr() const { return exportedFile; }

	inline void SetFile(const char* file) { fileName = file; }
	inline void SetExportedFile(const char* file) { exportedFile = file; }

	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	uint CountReferences() const;

	virtual void Save(Config& config) const;
	virtual void Load(const Config& config);
	virtual bool LoadInMemory() = 0;

protected:
	ResType type = None;
	UUID uuid = 0;
	std::string fileName;
	std::string exportedFile;	uint loaded = 0;

};

#endif