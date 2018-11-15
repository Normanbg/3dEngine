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
	

	bool ImportScene(const char* scene, std::vector<std::string>* written ); //imports .fbx to multiple .pei & textures (0.0)
	
	void LoadFBXandImportPEI(const char* FBXpath);//loads .fbx scene (1.0)
	ComponentMaterial* ImportMaterial(aiMaterial* mat); // loads a single texture (1.2)
	ComponentMesh* ImportMesh(aiMesh* mesh, const char* peiName); //loads a single mesh (1.2)

	//void LoadMeshPEI(ComponentMesh* mesh);
	void LoadMeshPEI(const char * fileNamePEI, ResourceMesh* resource); // loads a .pei into a resourceMesh

	
	void CleanUp();

private:
	bool ImportMeshRecursive(aiNode* node, const aiScene* scene, std::vector<std::string>* meshesNames); //imports .fbx meshes into .pei (0.1)

	GameObject* ImportNodeRecursive(aiNode* node, const aiScene* scene, GameObject* parent);//imports .fbx hierarchy (1.1)

};

#endif //__SCENEIMPORTER_H__
