#include "MeshImporter.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"


MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}

void MeshImporter::InitDebugLog(){
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	
}

void MeshImporter::EndDebugLog()
{
	aiDetachAllLogStreams();
}

void MeshImporter::LoadFBX(char * path){
	
	std::string rootPath = MODELS_PATH;
	rootPath += path;
	   	 
	const aiScene* scene = aiImportFile(rootPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr) {
		OWN_LOG("Error loading fbx from 3DModels folder.\n Loading FBX from HardDisk");
		scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
		if (scene == nullptr){
			OWN_LOG("Error loading scene %s", aiGetErrorString());
			aiReleaseImport(scene);
			return;
		}
	}
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
	Mesh mesh;

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
			}
			else {
				memcpy(&mesh.index[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}		
	}
	if (new_mesh->HasNormals()) {
		mesh.num_normals = new_mesh->mNumVertices;
		mesh.normals = new float3[mesh.num_normals];
		memcpy(mesh.normals, new_mesh->mNormals, sizeof(float3) * mesh.num_normals);
		OWN_LOG("New mesh with %d normals", mesh.num_normals);
	}	
	if (new_mesh->GetNumUVChannels() > 0) {
		mesh.num_textureCoords = mesh.num_vertex;
		mesh.texturesCoords = new float2[mesh.num_textureCoords];
		
		for (int i = 0; i < (mesh.num_textureCoords); i++) {
			mesh.texturesCoords[i].x = new_mesh->mTextureCoords[0][i].x;
			mesh.texturesCoords[i].y = new_mesh->mTextureCoords[0][i].y;
		}
		OWN_LOG("New mesh with %d UVs", mesh.num_textureCoords);
	}
	if (currSc->HasMaterials()) {
	
		aiColor3D color(0.f, 0.f, 0.f);
		currSc->mMaterials[new_mesh->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mesh.colors.x = color.r;
		mesh.colors.y = color.g;
		mesh.colors.z = color.b;		

		aiString path;
		aiReturn ret = currSc->mMaterials[new_mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (ret == aiReturn_SUCCESS) {

			std::string nwPath = TEXTURES_PATH;
			nwPath += path.C_Str();
			Texture _text;			
			_text.textureID = App->renderer3D->texImporter->LoadTexture(nwPath.c_str(),_text.texWidth,_text.texHeight);
			OWN_LOG("Loading texture from Textures folder");
			if (_text.textureID == -1) {
				OWN_LOG("Texture not found. \nLoading texture from HardDisk");
				
				_text.textureID = App->renderer3D->texImporter->LoadTexture(path.C_Str(), _text.texWidth, _text.texHeight);
				if (_text.textureID == -1) {

					OWN_LOG("Texture not found.\n Error loading texture from fbx.");
					return;
				}
			}
			mesh.texID = _text.textureID;
			App->renderer3D->AddTexture(&_text);
		}
		else {
			
			OWN_LOG("Error loading texture from fbx. Error: %s", aiGetErrorString());
		}
	}
	mesh.generateBoundingBox();
	App->renderer3D->AddMesh(&mesh);
}

void MeshImporter::ChangeMeshTexture(const char * path){
	uint width, height;
	std::vector<Mesh>* meshCopy = App->renderer3D->GetMeshesList();
	GLuint _texID = App->renderer3D->texImporter->LoadTexture(path, width, height);
	for (int i = 0; i < meshCopy->size(); i++) {
		(*meshCopy)[i].texID = _texID;
	}
	//App->renderer3D->meshes = meshCopy;
}

