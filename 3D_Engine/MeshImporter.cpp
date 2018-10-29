#include "MeshImporter.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ComponentTransformation.h"
#include "ComponentMesh.h"

#include "GameObject.h"
MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}
void LogCall(const char* msg, char* userData);
void MeshImporter::InitDebugLog(){
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCall;
	aiAttachLogStream(&stream);
	
}

void MeshImporter::EndDebugLog() const{
	aiDetachAllLogStreams();
}

void MeshImporter::LoadFBX(const char * path){
		   	 
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	//---checks if fbx path exist
	if (scene == nullptr) {
		OWN_LOG("Error loading scene %s", aiGetErrorString());
		aiReleaseImport(scene);
		return;

	}//------

	std::string modelName;

	GameObject* gameObject = App->scene->AddGameObject(modelName.c_str());

	App->fileSys->GetNameFromPath(path, nullptr, &modelName, nullptr);
	ComponentTransformation* compTrans = (ComponentTransformation*)gameObject->AddComponent(TRANSFORM);

	if (scene->HasMaterials()) {
		//need to load a texture outside the mesh. as a scene
		///check if material is in the path
		///if it is, load texture, convert to .dds & added as a component
		uint numMaterials = scene->mNumMaterials;
		for (int i = 0; i < numMaterials; i++) {

			aiMaterial* material = scene->mMaterials[i];

			Material* mat = new Material;

			aiColor3D color(0.f, 0.f, 0.f);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			mat->colors[0] = color.r;
			mat->colors[1] = color.g;
			mat->colors[2] = color.b;

			aiString texturePath;
			aiReturn ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
			if (ret == aiReturn_SUCCESS) {

				std::string fullTexPath = TEXTURES_PATH;
				fullTexPath += texturePath.C_Str();

				std::string textureName;
				std::string extension;
				App->fileSys->GetNameFromPath(fullTexPath.c_str(), nullptr, &textureName, &extension);

				GLuint check = App->textures->CheckIfImageAlreadyLoaded(textureName.c_str());
				if (check == -1) {
					if (extension != DDS_FORMAT) {
						App->renderer3D->texImporter->ImportToDDS(fullTexPath.c_str(), textureName.c_str());
					}
					std::string texDDSPath = LIB_TEXTURES_PATH + textureName + DDS_FORMAT;
					mat->textureID = App->renderer3D->texImporter->LoadTexture(texDDSPath.c_str(), mat);
					OWN_LOG("Loading texture from Lib/Textures folder");
					if (mat->textureID == -1) { // first check if texture is in local path "Lib/Textures"
						OWN_LOG("Error loading texture.");
					}
					if (mat->textureID != -1) { // if texture can be loaded
						mat->path = fullTexPath;
						//mesh.texID = _text.textureID;

						App->textures->AddTexture(mat);
					}
				}
				else {
					//mesh->texID = check;
				}
			}
			else {

				OWN_LOG("Error loading texture from fbx. Error: %s", aiGetErrorString());
			}
		}
	}
	
	if (scene->HasMeshes())	{
		OWN_LOG("Loading FBX mesh from %s", path);
		bool error = false;

		const aiNode* node = scene->mRootNode; // NEEDTO delete pointer?

		aiVector3D _scale;
		aiQuaternion _rotation;
		aiVector3D _position;
		node->mTransformation.Decompose(_scale, _rotation, _position);
		compTrans->scale = { _scale.x, _scale.y, _scale.z };
		compTrans->position = { _position.x, _position.y, _position.z };
		compTrans->rotation = { _rotation.x, _rotation.y, _rotation.z, _rotation.w };
		
		aiMesh * meshIterator = nullptr;
		for (int i = 0; i < scene->mNumMeshes; i++) {
			meshIterator = scene->mMeshes[i];
			
			ComponentMesh* compMesh = (ComponentMesh*)gameObject->AddComponent(MESH);

			
			compMesh->num_vertex = meshIterator->mNumVertices;
			compMesh->vertex = new float3[compMesh->num_vertex];
			memcpy(compMesh->vertex, meshIterator->mVertices, sizeof(float3) * compMesh->num_vertex);
			OWN_LOG("New mesh with %d vertices", compMesh->num_vertex);
			
			if (meshIterator->HasFaces()) {
				compMesh->num_index = meshIterator->mNumFaces * 3;				
				compMesh->index = new uint[compMesh->num_index]; // assume each face is a triangle
				for (uint i = 0; i < meshIterator->mNumFaces; ++i) {
					if (meshIterator->mFaces[i].mNumIndices != 3) {
						OWN_LOG("WARNING, geometry face with != 3 indices!");
						error = true;

					}
					else {
						memcpy(&compMesh->index[i * 3], meshIterator->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}
			if (meshIterator->HasNormals() && !error) {
				compMesh->num_normals = meshIterator->mNumVertices;
				compMesh->normals = new float3[compMesh->num_normals];
				memcpy(compMesh->normals, meshIterator->mNormals, sizeof(float3) * compMesh->num_normals);
				OWN_LOG("New mesh with %d normals", compMesh->num_normals);
			}
			if (meshIterator->GetNumUVChannels() > 0 && !error) {
				compMesh->num_textureCoords = compMesh->num_vertex;
				compMesh->texturesCoords = new float2[compMesh->num_textureCoords];

				for (int i = 0; i < (compMesh->num_textureCoords); i++) {
					compMesh->texturesCoords[i].x = meshIterator->mTextureCoords[0][i].x;
					compMesh->texturesCoords[i].y = meshIterator->mTextureCoords[0][i].y;
				}
				OWN_LOG("New mesh with %d UVs", compMesh->num_textureCoords);
			}					
			else {
				OWN_LOG("Error loading mesh");
			}
		}	
		node = nullptr;
	}
	else{
		OWN_LOG("Error loading FBX, scene has no meshes");		
	}
	aiReleaseImport(scene);
}
/*
void MeshImporter::LoadFromMesh(const aiScene* currSc, aiMesh * new_mesh){
	
	bool error= false;
	
	Mesh mesh;

	const aiNode* node = currSc->mRootNode;
	aiVector3D _scale;
	aiQuaternion _rotation;
	aiVector3D _position;
	node->mTransformation.Decompose(_scale, _rotation, _position);

	mesh.position = { _position.x, _position.y, _position.z };
	mesh.rotation = { _rotation.x, _rotation.y, _rotation.z, _rotation.w };
	mesh.scale = { _scale.x, _scale.y, _scale.z };
	
	mesh.num_vertex = new_mesh->mNumVertices;
	mesh.vertex = new float3[mesh.num_vertex];
	memcpy(mesh.vertex, new_mesh->mVertices, sizeof(float3) * mesh.num_vertex);
	OWN_LOG("New mesh with %d vertices", mesh.num_vertex);
	mesh.name = new_mesh->mName.C_Str();

	if (new_mesh->HasFaces()){
		mesh.num_index = new_mesh->mNumFaces * 3;
		mesh.num_faces = new_mesh->mNumFaces;
		mesh.index = new uint[mesh.num_index]; // assume each face is a triangle
		for (uint i = 0; i < new_mesh->mNumFaces; ++i)	{
			if (new_mesh->mFaces[i].mNumIndices != 3) {
				OWN_LOG("WARNING, geometry face with != 3 indices!");
				error = true;

			}
			else {
				memcpy(&mesh.index[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}
	if (new_mesh->HasNormals() && !error) {
		mesh.num_normals = new_mesh->mNumVertices;
		mesh.normals = new float3[mesh.num_normals];
		memcpy(mesh.normals, new_mesh->mNormals, sizeof(float3) * mesh.num_normals);
		OWN_LOG("New mesh with %d normals", mesh.num_normals);
	}	
	if (new_mesh->GetNumUVChannels() > 0 && !error) {
		mesh.num_textureCoords = mesh.num_vertex;
		mesh.texturesCoords = new float2[mesh.num_textureCoords];
		
		for (int i = 0; i < (mesh.num_textureCoords); i++) {
			mesh.texturesCoords[i].x = new_mesh->mTextureCoords[0][i].x;
			mesh.texturesCoords[i].y = new_mesh->mTextureCoords[0][i].y;
		}
		OWN_LOG("New mesh with %d UVs", mesh.num_textureCoords);
	}
	if (currSc->HasMaterials()&&!error) {
	
		aiColor3D color(0.f, 0.f, 0.f);
		currSc->mMaterials[new_mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mesh.colors.x = color.r;
		mesh.colors.y = color.g;
		mesh.colors.z = color.b;		

		aiString path;
		aiReturn ret = currSc->mMaterials[new_mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (ret == aiReturn_SUCCESS) {

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
			}
		}
		else {
			
			OWN_LOG("Error loading texture from fbx. Error: %s", aiGetErrorString());
		}
	}
	if (!error) {
		mesh.generateBoundingBox();
		App->renderer3D->AddMesh(&mesh);
	}
	else {
		OWN_LOG("Error loading mesh");
	}
}

void MeshImporter::ChangeMeshTexture(const char * path) {

	std::vector<Mesh>* meshCopy = App->renderer3D->GetMeshesList();

	GLuint check = App->renderer3D->CheckIfImageAlreadyLoaded(path);
	if (check == -1) {

		Texture tex;
		tex.textureID = App->renderer3D->texImporter->LoadTexture(path, tex.texWidth, tex.texHeight);
		tex.path = path;
		App->renderer3D->AddTexture(&tex);

		for (int i = 0; i < meshCopy->size(); i++) {
			(*meshCopy)[i].texID = tex.textureID;
		}
	}
	else {
		for (int i = 0; i < meshCopy->size(); i++) {
			(*meshCopy)[i].texID = check;
		}
	}
}
	//App->renderer3D->meshes = meshCopy;
*/

void LogCall(const char* msg, char* userData) {
	OWN_LOG("%s", msg);
}