#include "Importer.h"
#include "Globals.h"
#include "ModuleRenderer3D.h"


Importer::Importer()
{
}


Importer::~Importer()
{
}

void Importer::InitDebugLog(){
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	
}

void Importer::EndDebugLog()
{

	aiDetachAllLogStreams();
}

void Importer::LoadFBX(char * path)
{
	std::string rootPath = ROOT_PATH;
	rootPath += path;
	const aiScene* scene = aiImportFile(rootPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiMesh * meshIterator = nullptr;
		for (int i = 0; i < scene->mNumMeshes; i++) {
			meshIterator = scene->mMeshes[i];
			LoadFromMesh(meshIterator);
		}
		aiReleaseImport(scene);
	}
	else
		OWN_LOG("Error loading scene %s", aiGetErrorString());
}

void Importer::LoadFromMesh(aiMesh * new_mesh)
{
	Mesh mesh;


	mesh.num_vertex = new_mesh->mNumVertices;
	mesh.vertex = new float3[mesh.num_vertex];
	memcpy(mesh.vertex, new_mesh->mVertices, sizeof(float3) * mesh.num_vertex);
	OWN_LOG("New mesh with %d vertices", mesh.num_vertex);

	// copy faces
	if (new_mesh->HasFaces()){
		mesh.num_index = new_mesh->mNumFaces * 3;
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
	if (new_mesh->HasNormals())	{
		mesh.num_normals = new_mesh->mNumVertices;
		mesh.normals = new float3[mesh.num_normals]; 
		memcpy(mesh.normals, new_mesh->mNormals, sizeof(float3) * mesh.num_normals);
	}
	if (new_mesh->GetNumColorChannels() > 0) {
		mesh.num_colors = new_mesh->GetNumColorChannels();
		mesh.colors = new float3[mesh.num_colors];
		memcpy(mesh.colors, new_mesh->mColors, sizeof(float3) * mesh.num_colors);
	}
	if (new_mesh->GetNumUVChannels() > 0) {
		mesh.num_textureCoords = new_mesh->GetNumUVChannels();
		mesh.texturesCoords = new float2[mesh.num_textureCoords];
		memcpy(mesh.texturesCoords, new_mesh->mTextureCoords[0], sizeof(float2) * mesh.num_textureCoords);
	}
	

	App->renderer3D->AddMesh(&mesh);
}

