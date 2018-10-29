#ifndef __COMPONENTMESH_H__
#define __COMPONENTMESH_H__

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib\Math\MathAll.h"
#include "MathGeoLib/Geometry/AABB.h"


#include <array>
#include <vector>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"




class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

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

	uint texID = 0;

	AABB boundingBox;
	bool showBBox = false;

	void GenerateBoundingBox();
	void DrawBoundingBox();

	bool showWireframe = false;
	
	
	update_status PreUpdate(float dt);
	
	
	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	void GenerateBuffer();
	
	void Draw();

private:
	
};

#endif // !__COMPONENTMESH_H__
