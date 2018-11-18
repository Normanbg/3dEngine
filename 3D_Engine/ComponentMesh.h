#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "Globals.h"
#include "Math.h"
#include "ComponentMaterial.h"
#include "ComponentWithResource.h"

#include <array>
#include <vector>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


struct ComponentMaterial;
class ResourceMesh;

class ComponentMesh : public Component, public ComponentWithResource
{
public:
	ComponentMesh();
	~ComponentMesh();

	//void GenerateBoundingBox();
	void DrawBoundingBox();

	void CreateBBox();
	
	void SetMaterial(ComponentMaterial* texture);
	
	update_status PreUpdate(float dt);	
	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;


	void Save(Config& data) const override;
	void Load(Config* data) override;

	//void GenerateBuffer();

	void SetResource(uuid resource)override;
	Resource* GetResource() const override { return (Resource*)resourceMesh; }
	ResourceMesh* GetResourceMesh() const{ return resourceMesh; }

	const bool HasMesh() const;
	const bool HasMaterial() const;
	
	void Draw();

public:
	

	AABB bbox;
	
	bool showBBox = false;
	bool showWireframe = false;

	FrustumContained frustumContained = IS_OUT;

private:
	void DrawMesh();

private:
	ResourceMesh* resourceMesh = nullptr;
	ComponentMaterial* material = nullptr;

	bool drawWire = false;
};

#endif // !__COMPONENTMESH_H__
