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

		uint bytes = sizeof(uint);
		memcpy(cursor, &newScene.numMeshes, bytes);

		bytes = sizeof(float3);//sizeof float3
		cursor += bytes;
		memcpy(cursor, &newScene.scale, bytes);

		cursor += bytes;
		memcpy(cursor, &newScene.position, bytes);

		cursor += bytes;
		memcpy(cursor, &newScene.colors, bytes);

		cursor += bytes;
		bytes = sizeof(Quat);//sizeof float3
		memcpy(cursor, &newScene.rotation, bytes);

		dataFile.write(data, size);

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

void SceneImporter::ImportFromMesh(aiMesh * new_mesh, ofstream* dataFile)
{
	bool error = false;

	dataMesh newMesh;
	newMesh.num_vertex = new_mesh->mNumVertices; //-----------vertex
	newMesh.vertex = new float3[newMesh.num_vertex];
	memcpy(newMesh.vertex, new_mesh->mVertices, sizeof(float3) * newMesh.num_vertex);
	OWN_LOG("Importing new mesh with %d vertices", newMesh.num_vertex);

	if (new_mesh->HasFaces()) { //------------indices
		newMesh.num_index = new_mesh->mNumFaces * 3;		
		newMesh.index = new uint[newMesh.num_index]; // assume each face is a triangle
		for (uint i = 0; i < new_mesh->mNumFaces; ++i) {
			if (new_mesh->mFaces[i].mNumIndices != 3) {
				OWN_LOG("WARNING, geometry face with != 3 indices!");
				error = true;

			}
			else {
				memcpy(&newMesh.index[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}
	if (new_mesh->HasNormals() && !error) { //------------normals
		
		newMesh.normals = new float3[newMesh.num_vertex];
		memcpy(newMesh.normals, new_mesh->mNormals, sizeof(float3) * newMesh.num_vertex);
		
	}

}