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
		uint num_normals = 0;
		float3* normals = nullptr;
		uint num_texCoords = 0;
		float2* texturesCoords = nullptr;

		uint texID;
	};	

public:
	SceneImporter();
	~SceneImporter();

public:

	void Init();
	uint* ImportFBXtoPEI(const char* path);
	uint* newImportFBXtoPEI(const char* FBXpath);
	GameObject* ImportNodeRecursive(aiNode* node, const aiScene* scene, GameObject* parent);

	ComponentMaterial* ImportMaterial(aiMaterial* mat);
	ComponentMesh* ImportMesh(aiMesh* mesh);
	void ImportFBXandLoad(const char* fbxPath);
	void LoadPEI(const char* fileName, uint* texMeshLinker = nullptr);
	bool LoadMeshPEI(ComponentMesh* mesh);
	void CleanUp();

	void LoadFBX(const char * path);

	//uint* texIDs; //to store the id textures of the meshes

private:
	float4x4 aiMatrixToFloat4x4(aiMatrix4x4 matrix);
	aiMatrix4x4 savedMatrix = aiMatrix4x4();
	void ImportFromMesh(const aiScene* currSc, aiMesh * new_mesh, std::ofstream* dataFile, uint meshNum, uint* texIDs = nullptr, uint* texMeshIDs = nullptr);
};

#endif //__SCENEIMPORTER_H__
