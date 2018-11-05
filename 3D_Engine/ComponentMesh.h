#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "Globals.h"
#include "Math.h"
#include "ComponentMaterial.h"

#include <array>
#include <vector>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"


struct ComponentMaterial;

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void GenerateBoundingBox();
	void DrawBoundingBox();
	
	void SetMaterial(ComponentMaterial* texture);
	
	update_status PreUpdate(float dt);	
	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;


	void Save(Config& data) const override;
	void Load(Config* data) override;

	void GenerateBuffer();
	
	void Draw();

public:
	uint id_index = -1;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = -1;
	uint num_vertex = 0;
	float3* vertex = nullptr;

	uint id_normals = -1;
	uint num_normals = 0;
	float3* normals = nullptr;

	uint num_textureCoords = 0;
	float2* texturesCoords = nullptr;

	uint num_faces = 0;

	AABB boundingBox;
	bool showBBox = false;
	bool showWireframe = false;




private:
	ComponentMaterial* material = nullptr;
};

#endif // !__COMPONENTMESH_H__
