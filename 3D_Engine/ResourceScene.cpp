#include "ResourceScene.h"
#include "Config.h"


ResourceScene::ResourceScene(uuid UUID) : Resource(UUID, ResType::Scene)
{
}

ResourceScene::~ResourceScene()
{
}

void ResourceScene::LoadInMemory()
{
	
}

void ResourceScene::Save(Config & config) const
{
}

void ResourceScene::Load(const Config & config)
{
}

