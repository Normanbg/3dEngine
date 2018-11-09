#ifndef __SCENEIMPORTER_H__
#define __SCENEIMPORTER_H__

#include "Math.h"

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
	

	
public:
	SceneImporter();
	~SceneImporter();

public:

	void Init();
	


	void LoadFBXandImportPEI(const char* FBXpath);
	GameObject* ImportNodeRecursive(aiNode* node, const aiScene* scene, GameObject* parent);

	ComponentMaterial* ImportMaterial(aiMaterial* mat);
	ComponentMesh* ImportMesh(aiMesh* mesh, const char* peiName);
	void LoadMeshPEI(ComponentMesh* mesh);

	
	void CleanUp();

	void CreateBBox(ComponentMesh* newMesh);
	
	
private:
	float4x4 aiMatrixToFloat4x4(aiMatrix4x4 matrix);
	aiMatrix4x4 savedMatrix = aiMatrix4x4();
	
};

#endif //__SCENEIMPORTER_H__
