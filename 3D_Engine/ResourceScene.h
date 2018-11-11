#ifndef __RESOURCE_SCENE_H__
#define __RESOURCE_SCENE_H__

#include "Globals.h"
#include "Resource.h"

struct Config;

class ResourceScene : public Resource
{
	
public:
	ResourceScene(UUID uuid);
	virtual ~ResourceScene();

	bool LoadInMemory() override;
	void Save(Config& config) const override;
	void Load(const Config& config) override;

}
#endif