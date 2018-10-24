#ifndef __SCENEIMPORTER_H__
#define __SCENEIMPORTER_H__

#include "MathGeoLib\Math\MathAll.h"

#include <iostream>
#include <fstream> 



#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")



class SceneImporter
{
	struct dataScene {
		float3 position;
		float3 scale;
		Quat rotation;

		

		uint numMeshes;

	};

	struct dataMesh {
		
		float colors[3];

		uint num_index = 0;
		uint* index = nullptr;

		uint num_vertex = 0;
		float3* vertex = nullptr;
		float3* normals = nullptr;
		float2* texturesCoords = nullptr;
	};	

public:
	SceneImporter();
	~SceneImporter();

public:
	void ImportFBXtoPEI(const char* path, const char* name);
	void LoadPEI(const char* fileName);

private:
	void ImportFromMesh(const aiScene* currSc, aiMesh * new_mesh, std::ofstream* dataFile);
};

#endif //__SCENEIMPORTER_H__
