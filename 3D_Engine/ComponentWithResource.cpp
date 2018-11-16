#include "ComponentWithResource.h"
#include "Application.h"
#include "ModuleResources.h"


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
