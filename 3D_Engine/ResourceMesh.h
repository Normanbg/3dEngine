#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Globals.h"
#include "Resource.h"

struct Config;

class ResourceMesh : public Resource
{

public:
	ResourceMesh(UUID uuid);
	virtual ~ResourceMesh();

	bool LoadInMemory() override;
	void Save(Config& config) const override;
	void Load(const Config& config) override;

}
#endif