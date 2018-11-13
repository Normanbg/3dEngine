#include "Resource.h"
#include "Globals.h"


Resource::Resource(uuid UUID, ResType type) : UUID(UUID), type(type)
{
}

Resource::~Resource()
{
}

bool Resource::LoadToMemory()
{
	return false;
}

uint Resource::CountReferences() const
{
	
	return uint();
}
