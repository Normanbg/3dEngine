#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Resource.h"
#include "Globals.h"

class Resource;

class ResourceMesh : public Resource
{

public:
	ResourceMesh(uuid UUID);
	virtual ~ResourceMesh();

	bool LoadInMemory() override;
	void Save(Config& config) const override;
	void Load(const Config& config) override;

};
#endif// !__RESOURCE_MESH_H__