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

	void CheckMetaFiles();

	uuid FindByName(const char* name, Resource::ResType type = Resource::ResType::None) const;
	uuid FindByPath(const char* fileInAssets, Resource::ResType type = Resource::ResType::None) const;
	bool ImportFileAndGenerateMeta(const char* newFileInAssets);
	uuid GenerateNewUUID();
	const Resource* Get(uuid uuid) const;
	Resource* Get(uuid uuid);

	void LoadFiles(const char* filePath);
	
	std::map<uuid, Resource*> GetResourcesList() const { return resources; }
	std::vector<Resource*> GetResourcesListType(Resource::ResType type, bool loaded = false); // gets resources ResourceType "type" & possibilty to only take the loaded ones.

	Resource* CreateNewResource(Resource::ResType type, uuid forceUUID = 0);
	void RemoveResource(const char* path);
	void RemoveResource(uuid UUID);
	void GenerateMetaFile(const char* assetFile, uuid ResourceUUID, std::vector<uuid> exportedUUIDs);

	const Resource::ResType GetResourceTypeFromExtension(const char * path) const;
	void ChangeResourceUUID(uuid formerUUID, uuid newUUID);

private:

	uuid last_uuid = 1;
	std::map<uuid, Resource*> resources;
	Timer lastCheck;

private:

	bool ManageResourceWithMeta(const char* resource, const char* meta);
	
};

#endif // !__MODULE_RESOURCES_H__