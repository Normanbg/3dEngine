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

	UUID Find(const char* fileInAssets) const;
	UUID ImportFile(const char* newFileInAssets);
	UUID GenerateNewUUID();
	const Resource* Get(UUID uuid) const;
	Resource* Get(UUID uuid);
	Resource* CreateNewResource(Resource::ResType type, UUID forceUUID = 0);

	const Resource::ResType GetResourceTypeFromExtension(const char * path) const;

private:
	UUID last_uuid = 1;
	std::map<UUID, Resource*> resources;
	
};

#endif // !__MODULE_RESOURCES_H__