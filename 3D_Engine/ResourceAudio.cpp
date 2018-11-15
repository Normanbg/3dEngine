#include "ResourceAudio.h"
#include "Config.h"


ResourceAudio::ResourceAudio(uuid UUID) : Resource(UUID, ResType::Audio)
{
}

ResourceAudio::~ResourceAudio()
{
}

void ResourceAudio::LoadInMemory()
{
}

void ResourceAudio::UnloadInMemory()
{
}

void ResourceAudio::Save(Config & config) const
{
}

void ResourceAudio::Load(const Config & config)
{
}
