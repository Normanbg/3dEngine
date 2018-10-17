#include "MeshImporter.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"




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
	
	std::string rootPath = MODELS_PATH;
	rootPath += path;
	   	 
	const aiScene* scene = aiImportFile(rootPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	//---checks if fbx path exist
	if (scene == nullptr) {
		OWN_LOG("Error loading fbx from 3DModels folder.\n Loading FBX from HardDisk");
		scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
		if (scene == nullptr){
			OWN_LOG("Error loading scene %s", aiGetErrorString());
			aiReleaseImport(scene);
			return;
		}
	}//------
	
	
	if (scene->HasMeshes())	{
		OWN_LOG("Loading FBX mesh from %s", path);
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiMesh * meshIterator = nullptr;
		for (int i = 0; i < scene->mNumMeshes; i++) {
			meshIterator = scene->mMeshes[i];
			LoadFromMesh(scene, meshIterator);
		}	

	}
	else{
		OWN_LOG("Error loading FBX, scene has no meshes");		
	}
	aiReleaseImport(scene);
}

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


void LogCall(const char* msg, char* userData) {
	OWN_LOG("%s", msg);
}