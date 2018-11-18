#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__


#include "Module.h"
#include "Resource.h"


#include <map>

#define MS_TO_CHECK_META 2000


class Timer;

class ModuleResources : public Module
{
public:
	ModuleResources();
	~ModuleResources();

	bool Init(JSON_Object* obj) override;


	bool Start() override;
	update_status PreUpdate(float dt) override;
	bool CleanUp() override;

	Resource* CreateNewResource(Resource::ResType type, uuid forceUUID = 0);
	void RemoveResource(const char* path);
	void RemoveResource(uuid UUID);

	void ChangeResourceUUID(uuid formerUUID, uuid newUUID);
	void LoadFiles(const char* filePath);
	bool ImportFileAndGenerateMeta(const char* newFileInAssets);
	
	uuid FindByExportedFile(const char* name, Resource::ResType type = Resource::ResType::None) const;
	uuid FindByName(const char* name, Resource::ResType type = Resource::ResType::None) const;
	uuid FindByPath(const char* fileInAssets, Resource::ResType type = Resource::ResType::None) const;
	const Resource* Get(uuid uuid) const;
	Resource* Get(uuid uuid);	
	std::map<uuid, Resource*> GetResourcesList() const { return resources; }
	std::vector<Resource*> GetResourcesListType(Resource::ResType type, bool loaded = false); // gets resources ResourceType "type" & possibilty to only take the loaded ones.

	const Resource::ResType GetResourceTypeFromExtension(const char * path) const;
	

private:

	uuid last_uuid = 1;
	std::map<uuid, Resource*> resources;
	Timer lastCheck;

private:

	bool ManageResourceWithMeta(const char* resource, const char* meta);
	void GenerateMetaFile(const char* assetFile, uuid ResourceUUID, std::vector<uuid> exportedUUIDs);
	void CheckMetaFiles();
};

#endif // !__MODULE_RESOURCES_H__