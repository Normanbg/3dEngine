#ifndef __SCENEIMPORTER_H__
#define __SCENEIMPORTER_H__

#include "MathGeoLib\Math\MathAll.h"

#include <iostream>
#include <fstream> 

#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ModuleFileSystem.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

class ComponentMesh;

class SceneImporter
{
public:
	struct dataScene {
	
		float3 position = { 0,0,0 };
		float3 scale = { 0,0,0 };
		Quat rotation = Quat(0,0,0,0);
		
		uint numMeshes = 0;		

	};

	struct dataMesh {
	public:
		float colors[3];

		uint num_index = 0;
		uint* index = nullptr;

		uint num_vertex = 0;
		float3* vertex = nullptr;
		float3* normals = nullptr;
		float2* texturesCoords = nullptr;

		uint texID;
	};	

public:
	SceneImporter();
	~SceneImporter();

public:

	void Init();
	void ImportFBXtoPEI(const char* path);
	void LoadPEI(const char* fileName);
	void CleanUp();

	

private:
	void ImportFromMesh(const aiScene* currSc, aiMesh * new_mesh, std::ofstream* dataFile);
};

#endif //__SCENEIMPORTER_H__
