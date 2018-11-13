#include "ResourceAudio.h"
#include "Config.h"


ResourceAudio::ResourceAudio(uuid UUID) : Resource(UUID, ResType::Audio)
{
}

ResourceAudio::~ResourceAudio()
{
}

bool ResourceAudio::LoadInMemory()
{
	return false;
}

void ResourceAudio::Save(Config & config) const
{
}

void ResourceAudio::Load(const Config & config)
{
}
