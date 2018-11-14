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
	

	bool ImportScene(const char* scene, std::string* written );
	
	void LoadFBXandImportPEI(const char* FBXpath);
	

	ComponentMaterial* ImportMaterial(aiMaterial* mat);
	ComponentMesh* ImportMesh(aiMesh* mesh, const char* peiName);
	void LoadMeshPEI(ComponentMesh* mesh);
	void LoadMeshPEI(const char * fileNamePEI);

	
	void CleanUp();

private:
	bool ImportMeshRecursive(aiNode* node, const aiScene* scene);

	GameObject* ImportNodeRecursive(aiNode* node, const aiScene* scene, GameObject* parent);
};

#endif //__SCENEIMPORTER_H__
