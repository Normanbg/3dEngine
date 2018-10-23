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

struct NewMesh {

	uint id_index = -1;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = -1;
	uint num_vertex = 0;
	float3* vertex = nullptr;

	uint id_normals = -1;
	uint num_normals = 0;
	float3* normals = nullptr;

	uint num_faces = 0; // assume each face is a triangle
	const char* name;
	vec colors = { 0,0,0 };

	uint num_textureCoords = 0;
	float2* texturesCoords = nullptr;

	//GLuint texID = 0;

	AABB boundingBox;


};


class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	bool Update() override;

	void CleanUp() override;

};

#endif // !__COMPONENTMESH_H__
