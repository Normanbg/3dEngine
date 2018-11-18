#include "Resource.h"
#include "Globals.h"
#include "ModuleResources.h"

#include "mmgr/mmgr.h"

Resource::Resource(uuid UUID, ResType type) : UUID(UUID), type(type)
{
}

Resource::~Resource()
{
}

void Resource::SetUUID(uuid newUUID)
{
	App->resources->ChangeResourceUUID(UUID, newUUID);
	UUID = newUUID; 
}


