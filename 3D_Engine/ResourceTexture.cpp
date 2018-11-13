#include "ResourceTexture.h"
#include "Config.h"




ResourceTexture::ResourceTexture(uuid UUID) : Resource(UUID, ResType::Texture)
{
}

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadInMemory()
{
	return false;
}

void ResourceTexture::Save(Config & config) const
{
}

void ResourceTexture::Load(const Config & config)
{
}

