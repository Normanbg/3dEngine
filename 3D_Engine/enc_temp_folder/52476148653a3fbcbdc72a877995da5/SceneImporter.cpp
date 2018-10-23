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
	std::string fileName = name;
	fileName += OWN_FILE_FORMAT;

	std::ofstream dataFile(fileName.c_str());
	OWN_LOG("Creating PEI file");

	dataScene newScene;

	newScene.scale = { 0,0,0 };
	newScene.position = { 1,1,1 };
	newScene.colors = { 2,2,2 };
	newScene.rotation = { 3,3,3,3 };

	uint size = sizeof(uint) + sizeof(float3) * 3 + sizeof(Quat); // numMeshes+(scale+pos+color) + rotation in bytes
	char* data = new char[size];
	char* cursor = data;
	float3 transform[3] = { newScene.scale, newScene.position, newScene.colors };

	uint bytes = sizeof(transform);
	memcpy(data, transform, bytes);
	/*
	cursor += bytes;
	bytes = sizeof(newScene.position);
	memcpy(cursor, &newScene.position, bytes);

	cursor += bytes;
	bytes = sizeof(newScene.colors);
	memcpy(cursor, &newScene.colors, bytes);

	cursor += bytes;
	bytes = sizeof(newScene.rotation);
	memcpy(cursor, &newScene.rotation, bytes);
	*/
	dataFile.write(data, size);

	dataFile.close();


	// AND EXPORT TO /LIBRARY
	
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

		dataScene newScene;

		const aiNode* node = scene->mRootNode;

		aiVector3D _scale;
		aiQuaternion _rotation;
		aiVector3D _position;
		node->mTransformation.Decompose(_scale, _rotation, _position);
		newScene.scale = { _scale.x, _scale.y, _scale.z };
		newScene.position = { _position.x, _position.y, _position.z };
		newScene.rotation = { _rotation.x, _rotation.y, _rotation.z, _rotation.w };

		newScene.numMeshes = scene->mNumMeshes;

		uint size = sizeof(uint) + sizeof(float3) * 3 + sizeof(Quat); // numMeshes+(scale+pos+color) + rotation in bytes
		char* data = new char[size];
		char* cursor = data;

		uint bytes = sizeof(newScene.scale);
		memcpy(cursor, &newScene.scale, bytes);

		cursor += bytes;
		bytes = sizeof(newScene.position);
		memcpy(cursor, &newScene.position, bytes);

		cursor += bytes;
		bytes = sizeof(newScene.colors);
		memcpy(cursor, &newScene.colors, bytes);

		cursor += bytes;
		bytes = sizeof(newScene.rotation);
		memcpy(cursor, &newScene.rotation, bytes);

		dataFile << data;

		aiMesh * meshIterator = nullptr;
		for (int i = 0; i < newScene.numMeshes; i++) {
			meshIterator = scene->mMeshes[i];
			ImportFromMesh( meshIterator, &dataFile);
		}
		dataFile.close();
	}
	else {
		OWN_LOG("Error loading FBX, scene has no meshes");
	}
	aiReleaseImport(scene);
	

}

void SceneImporter::ImportFromMesh( aiMesh * new_mesh, ofstream* dataFile)
{
	dataMesh newMesh;
	*dataFile << "newTest" << endl;
}

