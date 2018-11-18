#include "ComponentWithResource.h"
#include "Application.h"
#include "ModuleResources.h"

#include "mmgr/mmgr.h"

ComponentWithResource::ComponentWithResource()
{
}


ComponentWithResource::~ComponentWithResource()
{
}


Resource * ComponentWithResource::GetResource() const
{
	return App->resources->Get(resourceUUID);
}
