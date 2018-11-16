#include "ModuleScene.h"
#include "GameObject.h"
#include "Globals.h"
#include "SceneImporter.h"
#include "Timer.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"

#include "mmgr/mmgr.h"

using namespace std;



SceneImporter::SceneImporter()
{
	
}


SceneImporter::~SceneImporter()
{
}

void LogCall(const char* msg, char* userData) {};

void SceneImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCall;
	aiAttachLogStream(&stream);

}

bool SceneImporter::ImportScene(const char * FBXpath, std::vector<std::string>* written, uuid forceUUID )
{
	bool ret = true;
	
	std::string modelName;

	App->fileSys->GetNameFromPath(FBXpath, nullptr, &modelName, nullptr, nullptr);

	const aiScene* scene = aiImportFile(FBXpath, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene == nullptr) {
		
		OWN_LOG("Error loading fbx from Assets/3DModels folder.");
		aiReleaseImport(scene);
		ret = false;
		return ret;
	}

	uint numMaterials = scene->mNumMaterials;
	uint* materialIDs = new uint[scene->mNumMaterials];

	if (ret && scene->HasMaterials() ) {
		OWN_LOG("Importing FBX texture to DDS from %s", FBXpath);
		for (int i = 0; i < numMaterials; i++) {

			aiMaterial* material = scene->mMaterials[i];

			

			aiString texturePath;
			aiReturn retu = material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
			if (retu == aiReturn_SUCCESS) {
				
				std::string textureName;
				std::string extension;
				App->fileSys->GetNameFromPath(texturePath.C_Str(), nullptr, &textureName, nullptr, nullptr);
				App->fileSys->GetNameFromPath(texturePath.C_Str(), nullptr, nullptr, nullptr, &extension);
								
				std::string texDDSPath;				
					if (extension != DDS_FORMAT) {
						std::string texAssetsPath = TEXTURES_PATH + textureName + extension;
						ret = App->texImporter->ImportToDDS(texAssetsPath.c_str(), textureName.c_str());
						if (!ret) {							
							ret = App->texImporter->ImportToDDS(texturePath.C_Str(), textureName.c_str()); 
						}
						texDDSPath = LIB_TEXTURES_PATH + textureName + DDS_FORMAT;

					}
					else {
						texDDSPath = TEXTURES_PATH + textureName + DDS_FORMAT;
						App->fileSys->CopyDDStoLib(texDDSPath.c_str(), nullptr);
					}
			}
			else {
				OWN_LOG("Error loading texture from fbx. Error: %s", aiGetErrorString());
			}
		}
	}
	if (ret && scene->HasMeshes()) {
		OWN_LOG("Importing FBX mesh to PEI from %s", FBXpath);
		

		//std::string fileName = LIB_MODELS_PATH + modelName + OWN_FILE_FORMAT;		
		//if (written) { (*written).push_back(fileName); }
		ret = ImportMeshRecursive(scene->mRootNode, scene, written, forceUUID);
		aiReleaseImport(scene);		
	}
	else {
		OWN_LOG("Error loading FBX, scene has no meshes");
		ret = false;
	}
	return ret;
}

bool SceneImporter::ImportMeshRecursive(aiNode * node, const aiScene * scene, std::vector<std::string>* meshesNames, uuid forceUUID)
{
	bool ret = true;

	if (node->mMetaData != nullptr) {
	
		
		if (node->mNumMeshes > 0){
			for (uint i = 0; i < node->mNumMeshes; i++)
			{
				bool error = false;

				std::string meshName = node->mName.C_Str();
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

				std::string uuid;
				if (forceUUID != 0) { uuid += std::to_string(forceUUID) + "~"; }
				std::string fileName = LIB_MODELS_PATH;
				fileName += uuid + meshName;
				fileName += OWN_FILE_FORMAT;
				std::ofstream dataFile(fileName.c_str(), std::fstream::out | std::fstream::binary);

				uint ranges[4] = { mesh->mNumFaces * 3,mesh->mNumVertices, mesh->mNumVertices,  mesh->mNumVertices };
				uint* index = nullptr;
				if (mesh->HasFaces()) { //------------indices
					
					index = new uint[ranges[0]]; // assume each face is a triangle
					for (uint i = 0; i < mesh->mNumFaces; ++i) {
						if (mesh->mFaces[i].mNumIndices != 3) {
							OWN_LOG("WARNING, geometry face with != 3 indices!");
							error = true;
						}
						else {
							memcpy(&index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
						}
					}
				}
				float2* texturesCoords = nullptr;
				if (mesh->GetNumUVChannels() > 0) { //------------textureCoords					
					texturesCoords = new float2[ranges[3]];
					for (int i = 0; i < ranges[3]; i++) {
						texturesCoords[i].x = mesh->mTextureCoords[0][i].x;
						texturesCoords[i].y = mesh->mTextureCoords[0][i].y;
					}
				}
				else { // no textureCoords
					ranges[3] = 0;
				}


				if (!error) {

					uint size = sizeof(ranges) + sizeof(uint)*ranges[0] + sizeof(float3)*ranges[1] + sizeof(float3)*ranges[2] + sizeof(float2) * ranges[3]; // numIndex + numVertex + index + vertex + normals + textureCoords

					char* data = new char[size];
					char* cursor = data;

					uint bytes = sizeof(ranges);
					memcpy(cursor, ranges, bytes);

					cursor += bytes;
					bytes = sizeof(uint)* ranges[0];
					memcpy(cursor, index, bytes);

					cursor += bytes;
					bytes = sizeof(float3)*  ranges[1];
					memcpy(cursor, mesh->mVertices, bytes);

					cursor += bytes;
					bytes = sizeof(float3)* ranges[2];
					memcpy(cursor, mesh->mNormals, bytes);

					cursor += bytes;
					bytes = sizeof(float2)*  ranges[3];
					memcpy(cursor, texturesCoords, bytes);

					dataFile.write(data, size);

					dataFile.close();
					RELEASE_ARRAY(data);
					cursor = nullptr;
					ret = true;

					meshesNames->push_back(fileName);
				}
				RELEASE_ARRAY(texturesCoords);
				RELEASE_ARRAY(index);
			}
		}
	}
	for (uint i = 0; i < node->mNumChildren; i++) // recursivity
	{
		ret &= ImportMeshRecursive(node->mChildren[i], scene, meshesNames, forceUUID);

	}
	return ret;
}

void SceneImporter::LoadFBXScene(const char * FBXpath)
{
	Timer loadTime;
	
	std::string fullFBXPath;// = MODELS_PATH;
	fullFBXPath += FBXpath;

	std::string modelName;

	App->fileSys->GetNameFromPath(fullFBXPath.c_str(), nullptr, &modelName, nullptr, nullptr);

	const aiScene* scene = aiImportFile(fullFBXPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene == nullptr) {
		scene = aiImportFile(FBXpath, aiProcessPreset_TargetRealtime_MaxQuality);
		if (scene == nullptr) {
			OWN_LOG("Error loading fbx from Assets/3DModels folder.");
			aiReleaseImport(scene);

			return;
		}
	}
	else {
		GameObject* GO;
		GO = App->scene->AddGameObject(modelName.c_str());		
		GameObject* GOchild = ImportNodeRecursive(scene->mRootNode, scene, GO);
		
		
		GOchild = nullptr;
		GO = nullptr;
		aiReleaseImport(scene);
		OWN_LOG("Loaded succesfully fbx from %s. Loading time: %.1fms", FBXpath, loadTime.ReadSec());
	}
}

GameObject * SceneImporter::ImportNodeRecursive(aiNode * node, const aiScene * scene, GameObject * parent)
{
	GameObject* nodeGO = nullptr;
	
	

	if (node->mMetaData != nullptr) { // to get the gameobject not a transform matrix
	
		nodeGO = new GameObject();
		nodeGO->name = node->mName.C_Str();
		nodeGO->SetParent(parent);		

		aiVector3D position;
		aiQuaternion rotation;
		aiVector3D scale;
		node->mTransformation.Decompose(scale, rotation, position);		
		
		nodeGO->transformComp->setPos(float3(position.x, position.y, position.z));
		nodeGO->transformComp->setScale(float3(scale.x, scale.y, scale.z));
		nodeGO->transformComp->setRotQuat(Quat(rotation.x, rotation.y, rotation.z, rotation.w));
		

		if (node->mNumMeshes > 0)
		{
			for (uint i = 0; i < node->mNumMeshes; i++)
			{
				ComponentMaterial* compMat = nullptr;
				ComponentMesh* compMesh = new ComponentMesh();
				

				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				
				if (material) {
					compMat = ImportMaterialToResource(material);		
					
					if (compMat != nullptr) {						
						nodeGO->AddComponent(compMat, MATERIAL);						
					}
					else {
						delete compMat;
					}
				}
								
				std::string meshName = nodeGO->name;
				
				compMesh = ImportMeshToResource(mesh,meshName.c_str());
				if (compMesh != nullptr) {
					nodeGO->AddComponent(compMesh, MESH);
					nodeGO->SetLocalAABB(compMesh->bbox);
					//compMesh->GenerateBuffer();
				}				
				if (compMesh&&compMat) {
					compMesh->SetMaterial(compMat);
				}
								
			}
		}
	}	
	if (!nodeGO) { nodeGO = parent; }
	for (uint i = 0; i < node->mNumChildren; i++) // recursivity
	{
		GameObject* child = ImportNodeRecursive(node->mChildren[i], scene, nodeGO);

	}
	return nodeGO;
}

ComponentMaterial * SceneImporter::ImportMaterialToResource(aiMaterial * material) // imports material to dds
{
	ComponentMaterial* mat = nullptr;
	bool error, col, mater;
	error = col = mater = false;

	aiString texturePath;
	mat = new ComponentMaterial();
	

	aiColor3D color = aiColor3D();
	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	if (!color.IsBlack()) {
		mat->colors = { color.r, color.g, color.b };
		col = true;
	}

	aiReturn ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
	if (ret == aiReturn_SUCCESS) {
		mater = true; 

		std::string path;
		std::string textureName;
		std::string extension;
		App->fileSys->GetNameFromPath(texturePath.C_Str(), &path, &textureName, nullptr, nullptr);
		App->fileSys->GetNameFromPath(texturePath.C_Str(), nullptr, nullptr, nullptr, &extension);

		uuid UUID = App->resources->FindByName(textureName.c_str(), Resource::ResType::Texture);
		if (UUID == 0) {
			if (App->resources->ImportFileAndGenerateMeta(texturePath.C_Str())) {
				mat->SetResource(App->resources->FindByName(textureName.c_str(),  Resource::ResType::Texture));
			}			
		}
		else {
			
			mat->SetResource(UUID);
		}		
	}
	else {

		OWN_LOG("Error loading texture from fbx. Error: %s", aiGetErrorString());
	}
	if (col == false && mater == false) {
		mat->CleanUp();
		RELEASE(mat);
	}
	
	return mat;
}

ComponentMesh * SceneImporter::ImportMeshToResource(aiMesh * mesh, const char* peiName)
{

	bool error = false;
	ComponentMesh* newMesh = new ComponentMesh();
	uuid resourceUUID = App->resources->FindByName(peiName, Resource::ResType::Mesh);
	if (resourceUUID != 0) {//checks if resource already exists
		newMesh->SetResource(resourceUUID);
		newMesh->CreateBBox();
	}
	else {/*
		newMesh->num_vertex = mesh->mNumVertices; //-----------vertex
		newMesh->vertex = new float3[newMesh->num_vertex];
		memcpy(newMesh->vertex, mesh->mVertices, sizeof(float3) * newMesh->num_vertex);
		OWN_LOG("Importing new mesh with %d vertices", newMesh->num_vertex);

		//texMeshIDs[meshNum] = new_mesh->mMaterialIndex;
		newMesh->CreateBBox(newMesh);

		if (mesh->HasFaces()) { //------------indices
			newMesh->num_index = mesh->mNumFaces * 3;
			newMesh->index = new uint[newMesh->num_index]; // assume each face is a triangle
			for (uint i = 0; i < mesh->mNumFaces; ++i) {
				if (mesh->mFaces[i].mNumIndices != 3) {
					OWN_LOG("WARNING, geometry face with != 3 indices!");
					error = true;

				}
				else {
					memcpy(&newMesh->index[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));

				}
			}
		}
		if (mesh->HasNormals()) { //------------normals
			newMesh->num_normals = newMesh->num_vertex;
			newMesh->normals = new float3[newMesh->num_normals];
			memcpy(newMesh->normals, mesh->mNormals, sizeof(float3) * newMesh->num_normals);
		}
		if (mesh->GetNumUVChannels() > 0) { //------------textureCoords
			newMesh->num_textureCoords = newMesh->num_vertex;
			newMesh->texturesCoords = new float2[newMesh->num_textureCoords];

			for (int i = 0; i < (newMesh->num_textureCoords); i++) {
				newMesh->texturesCoords[i].x = mesh->mTextureCoords[0][i].x;
				newMesh->texturesCoords[i].y = mesh->mTextureCoords[0][i].y;
			}
		}

		if (error) {
			newMesh->CleanUp();
			newMesh = nullptr;
			return nullptr;
		}
		else {

			std::string fileName = LIB_MODELS_PATH;
			fileName += peiName;
			fileName += OWN_FILE_FORMAT;
			std::ofstream dataFile(fileName.c_str(), std::fstream::out | std::fstream::binary);
			OWN_LOG("Importing file to PEI");

			uint ranges[4] = { newMesh->num_index,newMesh->num_vertex, newMesh->num_normals, newMesh->num_textureCoords };
			newMesh->num_faces = newMesh->num_index / 3;

			uint size = sizeof(ranges) + sizeof(uint)*newMesh->num_index + sizeof(float3)*newMesh->num_vertex + sizeof(float3)*newMesh->num_normals + sizeof(float2) * newMesh->num_textureCoords; // numIndex + numVertex + index + vertex + normals + textureCoords

			char* data = new char[size];
			char* cursor = data;

			uint bytes = sizeof(ranges);
			memcpy(cursor, ranges, bytes);

			cursor += bytes;
			bytes = sizeof(uint)* newMesh->num_index;
			memcpy(cursor, newMesh->index, bytes);

			cursor += bytes;
			bytes = sizeof(float3)* newMesh->num_vertex;
			memcpy(cursor, newMesh->vertex, bytes);

			cursor += bytes;
			bytes = sizeof(float3)* newMesh->num_normals;
			memcpy(cursor, newMesh->normals, bytes);

			cursor += bytes;
			bytes = sizeof(float2)* newMesh->num_textureCoords;
			memcpy(cursor, newMesh->texturesCoords, bytes);

			dataFile.write(data, size);

			dataFile.close();
			RELEASE_ARRAY(data);
			cursor = nullptr;
		}*/
	}
	return newMesh;
}
/*

void SceneImporter::LoadMeshPEI(ComponentMesh* compMesh) {
	
	Timer loadingTimer;

	std::string filePath = LIB_MODELS_PATH;
	filePath += compMesh->myGO->name + OWN_FILE_FORMAT;

	

	std::ifstream dataFile(filePath.c_str(), std::fstream::out | std::fstream::binary);
	if (dataFile.fail()) {
		OWN_LOG("Error loading PEI. Cannot find PEI file in %s in Lib/Textures", filePath.c_str());
		return;
	}

	
		

		uint ranges[4] = { 0,0,0,0 }; // [numIndex , numVertex, numNormals, numTexCoords]

		uint rangesSize = sizeof(ranges);
		char* headerdata = new char[rangesSize];
			

		dataFile.read(headerdata, rangesSize);		
		memcpy(ranges, headerdata, rangesSize);

		compMesh->num_index = ranges[0];
		compMesh->num_vertex = ranges[1];
		compMesh->num_normals = ranges[2];
		compMesh->num_textureCoords = ranges[3];
		compMesh->num_faces = compMesh->num_index / 3;

		compMesh->index = new uint[compMesh->num_index];
		compMesh->vertex = new float3[compMesh->num_vertex];
		compMesh->normals = new float3[compMesh->num_normals];
		compMesh->texturesCoords = new float2[compMesh->num_textureCoords];

		uint meshDataSize = sizeof(uint)* compMesh->num_index + sizeof(float3)*compMesh->num_vertex + sizeof(float3)*compMesh->num_normals + sizeof(float2) * compMesh->num_textureCoords;

		char* meshdata = new char[meshDataSize];
		char* mcursor = meshdata;


		dataFile.seekg(rangesSize);

		dataFile.read(meshdata, meshDataSize);


		uint mbytes = sizeof(uint) *compMesh->num_index;//index		
		memcpy(compMesh->index, mcursor, mbytes);

		mcursor += mbytes;
		mbytes = sizeof(float3)*compMesh->num_vertex;//vertex		
		memcpy(compMesh->vertex, mcursor, mbytes);

		mcursor += mbytes;
		mbytes = sizeof(float3)*compMesh->num_normals;//normals	
		memcpy(compMesh->normals, mcursor, mbytes);

		mcursor += mbytes;
		mbytes = sizeof(float2)*compMesh->num_textureCoords; //texCoords
		memcpy(compMesh->texturesCoords, mcursor, mbytes);

		
		compMesh->GenerateBuffer();

		OWN_LOG("Succesfully loaded PEI: %s. Loading time: %i ms \n", compMesh->myGO->name.c_str(), loadingTimer.Read());

		RELEASE_ARRAY(headerdata);
		RELEASE_ARRAY(meshdata);
		mcursor = nullptr;
		compMesh = nullptr;

	
	
	dataFile.close();
}
*/
void SceneImporter::LoadMeshPEI(const char* fileNamePEI, ResourceMesh* resource) {
	
	Timer loadingTimer;
	std::string peiName;
	App->fileSys->GetNameFromPath(fileNamePEI, nullptr, &peiName, nullptr, nullptr);
	std::string filePath = LIB_MODELS_PATH;
	filePath += peiName;
	filePath += OWN_FILE_FORMAT;

	

	std::ifstream dataFile(filePath.c_str(), std::fstream::out | std::fstream::binary);
	if (dataFile.fail()) {
		OWN_LOG("Error loading PEI. Cannot find PEI file in %s in Lib/Textures", filePath.c_str());
		return;
	}

	
	uint ranges[4] = { 0,0,0,0 }; // [numIndex , numVertex, numNormals, numTexCoords]

	uint rangesSize = sizeof(ranges);
	char* headerdata = new char[rangesSize];


	dataFile.read(headerdata, rangesSize);
	memcpy(ranges, headerdata, rangesSize);

	resource->num_index = ranges[0];
	resource->num_vertex = ranges[1];
	resource->num_normals = ranges[2];
	resource->num_textureCoords = ranges[3];

	resource->index = new uint[resource->num_index];
	resource->vertex = new float3[resource->num_vertex];
	resource->normals = new float3[resource->num_normals];
	resource->texturesCoords = new float2[resource->num_textureCoords];
	resource->num_faces = resource->num_index / 3;

	uint meshDataSize = sizeof(uint)* resource->num_index + sizeof(float3)*resource->num_vertex + sizeof(float3)*resource->num_normals + sizeof(float2) * resource->num_textureCoords;

	char* meshdata = new char[meshDataSize];
	char* mcursor = meshdata;


	dataFile.seekg(rangesSize);

	dataFile.read(meshdata, meshDataSize);


	uint mbytes = sizeof(uint) *resource->num_index;//index		
	memcpy(resource->index, mcursor, mbytes);

	mcursor += mbytes;
	mbytes = sizeof(float3)*resource->num_vertex;//vertex		
	memcpy(resource->vertex, mcursor, mbytes);

	mcursor += mbytes;
	mbytes = sizeof(float3)*resource->num_normals;//normals	
	memcpy(resource->normals, mcursor, mbytes);

	mcursor += mbytes;
	mbytes = sizeof(float2)*resource->num_textureCoords; //texCoords
	memcpy(resource->texturesCoords, mcursor, mbytes);
	   
	OWN_LOG("Succesfully loaded PEI: %s. Loading time: %i ms \n", fileNamePEI, loadingTimer.Read());

	RELEASE_ARRAY(headerdata);
	RELEASE_ARRAY(meshdata);
	mcursor = nullptr;

	dataFile.close();
}

void SceneImporter::CleanUp()
{
	aiDetachAllLogStreams();
}
