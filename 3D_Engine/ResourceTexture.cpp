#include "ResourceTexture.h"
#include "Config.h"




ResourceTexture::ResourceTexture(uuid UUID) : Resource(UUID, ResType::Texture)
{
}

ResourceTexture::~ResourceTexture()
{
}

void ResourceTexture::LoadInMemory()
{

}

void ResourceTexture::Save(Config & config) const
{
}

void ResourceTexture::Load(const Config & config)
{
}
