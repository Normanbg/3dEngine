#ifndef __SCENEIMPORTER_H__
#define __SCENEIMPORTER_H__

#include "MathGeoLib\Math\MathAll.h"
#include "Globals.h"

#include <iostream>
#include <fstream> 
using namespace std;


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

		float3 colors = { 0,0,0 };

		uint numMeshes;

	};
	struct dataMesh {
		

		uint num_index = 0;
		uint* index = nullptr;

		uint num_vertex = 0;
		float3* vertex = nullptr;
		float3* normals = nullptr;
		float3* texturesCoords = nullptr;
	};

	

public:
	SceneImporter();
	~SceneImporter();

public:
	void LoadFBXtoPEI(const char* path, const char* name);

private:
	void ImportFromMesh (aiMesh * new_mesh, ofstream* dataFile);
};

#endif //__SCENEIMPORTER_H__
