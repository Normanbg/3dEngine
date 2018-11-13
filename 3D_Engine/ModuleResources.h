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

	uuid Find(const char* fileInAssets) const;
	uuid ImportFile(const char* newFileInAssets);
	uuid GenerateNewUUID();
	const Resource* Get(uuid uuid) const;
	Resource* Get(uuid uuid);
	Resource* CreateNewResource(Resource::ResType type, uuid forceUUID = 0);

	const Resource::ResType GetResourceTypeFromExtension(const char * path) const;

private:

	uuid last_uuid = 1;
	std::map<uuid, Resource*> resources;
	
};

#endif // !__MODULE_RESOURCES_H__