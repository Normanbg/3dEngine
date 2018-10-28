

#include "ModuleScene.h"
#include "GameObject.h"

#include "Globals.h"
#include "SceneImporter.h"
#include "Timer.h"

#include <string>


using namespace std;



SceneImporter::SceneImporter()
{
	
}


SceneImporter::~SceneImporter()
{
}

void LogCall(const char* msg, char* userData);

void SceneImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCall;
	aiAttachLogStream(&stream);

}

void SceneImporter::ImportFBXtoPEI(const char * FBXpath)
{

	
	std::string fullFBXPath = MODELS_PATH;
	fullFBXPath+= FBXpath;
		
	std::string modelName;
	
	App->fileSys->GetNameFromPath(fullFBXPath.c_str(),nullptr, &modelName, nullptr);

	const aiScene* scene = aiImportFile(fullFBXPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality); 

	if (scene == nullptr) {		
		 scene = aiImportFile(FBXpath, aiProcessPreset_TargetRealtime_MaxQuality);
		 if (scene == nullptr) {
			 OWN_LOG("Error loading fbx from Assets/3DModels folder.");
			 aiReleaseImport(scene);
			 return;
		 }	
	}

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
					mat->textureID = App->renderer3D->texImporter->LoadTexture(texDDSPath.c_str(),mat);
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
	if (scene->HasMeshes()) {
		OWN_LOG("Importing FBX mesh to PEI from %s", FBXpath);
		// Use scene->mNumMeshes to iterate on scene->mMeshes array

		std::string fileName = LIB_MODELS_PATH + modelName + OWN_FILE_FORMAT;
		

		std::ofstream dataFile(fileName.c_str(), std::fstream::out | std::fstream::binary);
		OWN_LOG("Creating PEI file");

		SceneImporter::dataScene newScene;

		const aiNode* node = scene->mRootNode; // NEEDTO delete pointer?

		aiVector3D _scale;
		aiQuaternion _rotation;
		aiVector3D _position;
		node->mTransformation.Decompose(_scale, _rotation, _position);
		newScene.scale = { _scale.x, _scale.y, _scale.z };
		newScene.position = { _position.x, _position.y, _position.z };
		newScene.rotation = { _rotation.x, _rotation.y, _rotation.z, _rotation.w };

		newScene.numMeshes = scene->mNumMeshes;


		uint size = sizeof(uint) +sizeof(float3) * 2 + sizeof(Quat); // numMeshes+(scale+pos) + rotation in bytes
		char* data = new char[size];
		char* cursor = data;

		uint bytes = sizeof(uint); // 4bytes
		memcpy(cursor, &newScene.numMeshes, bytes); // copies 4B from cursor
		
		cursor += bytes; 
		bytes = sizeof(float3);//sizeof float3		
		memcpy(cursor, &newScene.scale, bytes);

		cursor += bytes;//sizeof float3		
		memcpy(cursor, &newScene.position, bytes);

		
		cursor += bytes;
		bytes = sizeof(Quat);//sizeof float3
		memcpy(cursor, &newScene.rotation, bytes);

		dataFile.write(data, size);

		delete[] data;
		data = nullptr;
		cursor = nullptr;

		aiMesh * meshIterator = nullptr; // NEEDTO delete pointer?

		for (int i = 0; i < newScene.numMeshes; i++) {
			meshIterator = scene->mMeshes[i];
			ImportFromMesh(scene, meshIterator, &dataFile);
		}
		
		aiReleaseImport(scene);		
		dataFile.close();

		
	}
	else {
		OWN_LOG("Error loading FBX, scene has no meshes");
	}
	
	

}

void SceneImporter::ImportFBXandLoad(const char * fbxPath)  
{
	ImportFBXtoPEI(fbxPath);

	std::string PEIPath;
	App->fileSys->GetNameFromPath(fbxPath, nullptr, &PEIPath, nullptr);
	PEIPath += OWN_FILE_FORMAT;
	
	LoadPEI(PEIPath.c_str());
}


void SceneImporter::ImportFromMesh(const aiScene* currSc, aiMesh * new_mesh,std::ofstream* dataFile)
{
	bool error = false;

	SceneImporter::dataMesh newMesh;
	newMesh.num_vertex = new_mesh->mNumVertices; //-----------vertex
	newMesh.vertex = new float3[newMesh.num_vertex];
	memcpy(newMesh.vertex, new_mesh->mVertices, sizeof(float3) * newMesh.num_vertex);
	OWN_LOG("Importing new mesh with %d vertices", newMesh.num_vertex);
	
	newMesh.texID = new_mesh->mMaterialIndex;

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
	
	
	if (!error) { // writting into file
	
		uint ranges[2] = { newMesh.num_index, newMesh.num_vertex };

		uint size = sizeof(ranges) + sizeof(uint)*newMesh.num_index + sizeof(float3)*newMesh.num_vertex*2+ sizeof(float2) * newMesh.num_vertex; // numIndex + numVertex + index + vertex + normals + textureCoords
		
		char* data = new char[size];
		char* cursor = data;

		uint bytes = sizeof(ranges);
		memcpy(cursor, ranges, bytes);

		cursor += bytes;
		bytes = sizeof(uint)* newMesh.num_index;		
		memcpy(cursor, newMesh.index, bytes);
		
		cursor += bytes;
		bytes = sizeof(float3)* newMesh.num_vertex;
		memcpy(cursor, newMesh.vertex, bytes);

		cursor += bytes;
		memcpy(cursor, newMesh.normals, bytes);
		
		cursor += bytes;
		bytes = sizeof(float2)* newMesh.num_vertex;		
		memcpy(cursor, newMesh.texturesCoords, bytes);
		
		dataFile->write(data, size);


		delete[] newMesh.index;
		delete[] newMesh.vertex;
		delete[] newMesh.normals;
		delete[] newMesh.texturesCoords;
		delete[] data;
		data = nullptr;
		cursor = nullptr;
	}
}

void SceneImporter::LoadPEI(const char * fileName)
{
	Timer loadingTimer;
	std::string modelName;

	App->fileSys->GetNameFromPath(fileName, nullptr, &modelName, nullptr);

	GameObject* gameObject = App->scene->AddGameObject(modelName.c_str());
	
	
	ComponentTransformation* compTrans = (ComponentTransformation*)gameObject->AddComponent(TRANSFORM);
	
	std::string filePath = LIB_MODELS_PATH;
	filePath += fileName;
	
	uint size = sizeof(uint) + sizeof(float3) * 2 + sizeof(Quat);

	std::ifstream dataFile(filePath.c_str(), std::fstream::out | std::fstream::binary );
	if (dataFile.fail()) {
		OWN_LOG("Error loading PEI. Cannot find PEI file %s", fileName);
		return;
	}

	char* scenedata =	new char[size];
	char* cursor = scenedata;
	dataFile.read(scenedata, size);
	
	uint numMeshes = 0;
	uint bytes = sizeof(uint);
	memcpy(&numMeshes, cursor, bytes);

	cursor += bytes;	
	bytes = sizeof(float3);
	memcpy(&compTrans->scale, cursor, bytes);

	cursor += bytes;
	
	memcpy(&compTrans->position, cursor, bytes);

	cursor += bytes;
	bytes = sizeof(Quat);//sizeof quat
	memcpy(&compTrans->rotation, cursor, bytes);
	
	delete[] scenedata;
	scenedata = nullptr;
	cursor = nullptr;

	

	uint totalSize = 0;
	totalSize = size;

	for (int i = 0; i < numMeshes; i++) {
		
		ComponentMesh* compMesh = (ComponentMesh*)gameObject->AddComponent(MESH);

				
		//---read ranges
		uint ranges[2]; // [numIndex , numVertex]

		uint rangesSize = sizeof(ranges); 

		char* headerdata = new char[rangesSize];		

		dataFile.seekg(totalSize);

		dataFile.read(headerdata, rangesSize);

		uint mbytes = sizeof(ranges);
		memcpy(ranges, headerdata, mbytes); 

		delete[] headerdata;
		headerdata = nullptr;
		//---

		compMesh->num_index = ranges[0];
		compMesh->num_vertex = compMesh->num_textureCoords = compMesh->num_normals = ranges[1];
	
		compMesh->index = new uint[compMesh->num_index];
		compMesh->vertex = new float3[compMesh->num_vertex];
		compMesh->normals = new float3[compMesh->num_vertex];
		compMesh->texturesCoords = new float2[compMesh->num_vertex];

		uint meshDataSize = sizeof(uint)* compMesh->num_index + sizeof(float3)*compMesh->num_vertex* 2 + sizeof(float2) * compMesh->num_vertex;

		char* meshdata = new char[meshDataSize];
		char* mcursor = meshdata;

				
		dataFile.seekg(totalSize+rangesSize);

		dataFile.read(meshdata, meshDataSize);
			

		mbytes = sizeof(uint) *compMesh->num_index;//index		
		memcpy(compMesh->index, mcursor, mbytes);
		
		mcursor += mbytes;
		mbytes = sizeof(float3)*compMesh->num_vertex;//vertex		
		memcpy(compMesh->vertex,mcursor , mbytes);
		
		mcursor += mbytes;
		memcpy(compMesh->normals, mcursor, mbytes);

		mcursor += mbytes;
		mbytes = sizeof(float2)*compMesh->num_vertex;
		memcpy(compMesh->texturesCoords,mcursor, mbytes);

		totalSize += rangesSize + meshDataSize;

		compMesh->GenerateBuffer();
		
		delete[] meshdata;
		meshdata = nullptr;
		mcursor = nullptr;
		compMesh = nullptr;
		
	}
	OWN_LOG("Succesfully loaded PEI: %s. Loading time: %i ms", modelName.c_str(), loadingTimer.Read());
	gameObject = nullptr;

	dataFile.close();
}

void SceneImporter::CleanUp()
{
	aiDetachAllLogStreams();
}

