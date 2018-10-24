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
#pragma comment (lib, "Assimp/libx86/assimp.lib")



class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	uint num_index = 0;
	uint* index = nullptr;

	uint num_vertex = 0;
	float3* vertex = nullptr;
	
	uint num_normals = 0;
	float3* normals = nullptr;

	uint num_textureCoords = 0;
	float2* texturesCoords = nullptr;

	AABB boundingBox;

	bool Update() override;

	void CleanUp() override;

};

#endif // !__COMPONENTMESH_H__
