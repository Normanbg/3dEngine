#include "SceneImporter.h"
#include "Globals.h"




SceneImporter::SceneImporter()
{
}


SceneImporter::~SceneImporter()
{
}

void SceneImporter::ImportFBXtoPEI(const char * FBXpath, const char* name)
{
	//NEED TO LOAD FROM /ASSETS in a file system module
	
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


		uint size = sizeof(uint) + sizeof(float3) * 2 + sizeof(Quat); // numMeshes+(scale+pos) + rotation in bytes
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
		bytes = sizeof(Quat);//sizeof float3
		memcpy(cursor, &newScene.rotation, bytes);

		dataFile.write(data, size);

		aiMesh * meshIterator = nullptr;
		for (int i = 0; i < newScene.numMeshes; i++) {
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


void SceneImporter::ImportFromMesh(const aiScene* currSc, aiMesh * new_mesh, ofstream* dataFile)
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
	if (new_mesh->GetNumUVChannels() > 0 && !error) { //------------textureCoords
		newMesh.texturesCoords = new float2[newMesh.num_vertex];

		for (int i = 0; i < (newMesh.num_vertex); i++) {
			newMesh.texturesCoords[i].x = new_mesh->mTextureCoords[0][i].x;
			newMesh.texturesCoords[i].y = new_mesh->mTextureCoords[0][i].y;
		}
	}

	if (currSc->HasMaterials() && !error) {

		aiMaterial* material = currSc->mMaterials[new_mesh->mMaterialIndex];

		aiColor3D color(0.f, 0.f, 0.f);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		newMesh.colors[0] = color.r;
		newMesh.colors[1] = color.g;
		newMesh.colors[2] = color.b;

		aiString path;
		aiReturn ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (ret == aiReturn_SUCCESS) {
			/* //check if material is in the path
			//if it is, load texture, convert to .dds & added as a component

			std::string localPath = TEXTURES_PATH;
			localPath += path.C_Str();
			GLuint check = App->renderer3D->CheckIfImageAlreadyLoaded(localPath.c_str());
			if (check == -1) {
				Texture _text;
				_text.textureID = App->renderer3D->texImporter->LoadTexture(localPath.c_str(), _text.texWidth, _text.texHeight);
				OWN_LOG("Loading texture from Textures folder");
				if (_text.textureID == -1) { // fisrt check if texture is in local path "Assets/Textures"
					OWN_LOG("Texture not found. \nLoading texture from HardDisk");

					_text.textureID = App->renderer3D->texImporter->LoadTexture(path.C_Str(), _text.texWidth, _text.texHeight);
					if (_text.textureID == -1) {// second check if texture is in absolute path C:\\...

						OWN_LOG("Texture not found in the HardDrisk.\n Error loading texture from fbx.");
					}

				}
				if (_text.textureID != -1) { // if texture can be loaded
					_text.path = localPath;
					mesh.texID = _text.textureID;
					App->renderer3D->AddTexture(&_text);
				}

			}
		else {
			mesh.texID = check;
		}*/
		}
		else {

			OWN_LOG("Error loading texture from fbx. Error: %s", aiGetErrorString());
		}
	}
	if (!error) { // writting into file
	
		uint ranges[2] = { newMesh.num_index, newMesh.num_vertex };

		uint size = sizeof(ranges) + sizeof(float)*3 + sizeof(uint)*newMesh.num_index + sizeof(float3)*newMesh.num_vertex*2+ sizeof(float2) * newMesh.num_vertex; // numIndex + numVertex + colors + index + vertex + normals + textureCoords
		
		char* data = new char[size];
		char* cursor = data;

		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);

		bytes = sizeof(float)*3;//sizeof float[3]
		cursor += bytes;
		memcpy(cursor, newMesh.colors, bytes);

		bytes = sizeof(uint)* newMesh.num_index;
		cursor += bytes;
		memcpy(cursor, newMesh.index, bytes);
	
		bytes = sizeof(float3)* newMesh.num_vertex;
		cursor += bytes;
		memcpy(cursor, newMesh.vertex, bytes);

		cursor += bytes;
		memcpy(cursor, newMesh.normals, bytes);
		
		bytes = sizeof(float2)* newMesh.num_vertex;
		cursor += bytes;
		memcpy(cursor, newMesh.texturesCoords, bytes);
		
		dataFile->write(data, size);
	}
}
void SceneImporter::LoadPEItoComponent(const char * fileName)
{
	std::ifstream dataFile(fileName);

	//dataFile.read();
}
