#include "ResourceMesh.h"
#include "Config.h"

ResourceMesh::ResourceMesh(uuid UUID) : Resource(UUID, ResType::Mesh)
{
}

ResourceMesh::~ResourceMesh()
{
}

bool ResourceMesh::LoadInMemory()
{
	return false;
}

void ResourceMesh::Save(Config & config) const
{
}

void ResourceMesh::Load(const Config & config)
{
}
