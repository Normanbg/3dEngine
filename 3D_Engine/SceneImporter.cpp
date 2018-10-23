#include "SceneImporter.h"
#include "Globals.h"



SceneImporter::SceneImporter()
{
}


SceneImporter::~SceneImporter()
{
}

void SceneImporter::LoadFBXtoPEI(const char * FBXpath, const char* name)
{
	//NEED TO LOAD FROM /ASSETS
	// AND EXPORT TO /LIBRARY
	std::string fileName = name;
	fileName += OWN_FILE_FORMAT;

	std::ofstream dataFile(fileName.c_str());
	ImportFromMesh(nullptr, nullptr, &dataFile);

	std::string rootPath = MODELS_PATH;
	rootPath += FBXpath;

	const aiScene* scene = aiImportFile(rootPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene == nullptr) {
		OWN_LOG("Error loading fbx from Assets/3DModels folder.");
		return;
	}

	if (scene->HasMeshes()) {
		OWN_LOG("Importing FBX mesh to PEI from %s", FBXpath);
		// Use scene->mNumMeshes to iterate on scene->mMeshes array

		std::string fileName = name;
		fileName += OWN_FILE_FORMAT;

		std::ofstream dataFile(fileName.c_str());
		OWN_LOG("Creating PEI file");

		aiMesh * meshIterator = nullptr;
		for (int i = 0; i < scene->mNumMeshes; i++) {
			meshIterator = scene->mMeshes[i];
			ImportFromMesh(scene, meshIterator, &dataFile);
		}
		dataFile.close();
	}
	else {
		OWN_LOG("Error loading FBX, scene has no meshes");
	}
	aiReleaseImport(scene);
	

}

void SceneImporter::ImportFromMesh(const aiScene * currSc, aiMesh * new_mesh, ofstream* dataFile)
{
	*dataFile << "newTest" << endl;
}

