#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__


#include "Module.h"
#include "Resource.h"

#include <map>

class ModuleResources : public Module
{
public:
	ModuleResources();
	~ModuleResources();

	bool Init(JSON_Object* obj) override;


	bool Start() override;
	bool CleanUp() override;

	void ImportFilesToLibrary();

	uuid FindByName(const char* name, Resource::ResType type = Resource::ResType::None) const;
	uuid FindByPath(const char* fileInAssets, Resource::ResType type = Resource::ResType::None) const;
	bool ImportFile(const char* newFileInAssets);
	uuid GenerateNewUUID();
	const Resource* Get(uuid uuid) const;
	Resource* Get(uuid uuid);
	std::map<uuid, Resource*> GetResourcesList() const { return resources; }
	std::vector<Resource*> GetResourcesListType(Resource::ResType type, bool loaded = false); // gets resources ResourceType "type" & possibilty to only take the loaded ones.

	Resource* CreateNewResource(Resource::ResType type, uuid forceUUID = 0);
	void GenerateMetaFile(const char* assetFile, std::vector<uuid> exportedUUIDs);

	const Resource::ResType GetResourceTypeFromExtension(const char * path) const;

private:

	uuid last_uuid = 1;
	std::map<uuid, Resource*> resources;
	
};

#endif // !__MODULE_RESOURCES_H__