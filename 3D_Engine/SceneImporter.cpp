

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

void LogCall(const char* msg, char* userData) {};

void SceneImporter::Init()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	stream.callback = LogCall;
	aiAttachLogStream(&stream);

}

void SceneImporter::LoadFBXandImportPEI(const char * FBXpath)
{
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
		nodeGO->transformComp->UpdateLocalMatrix();
		nodeGO->transformComp->localMatrix = nodeGO->transformComp->localMatrix * aiMatrixToFloat4x4(savedMatrix);
		savedMatrix = aiMatrix4x4();

		if (node->mNumMeshes > 0)
		{
			for (uint i = 0; i < node->mNumMeshes; i++)
			{
				ComponentMaterial* compMat = nullptr;
				ComponentMesh* compMesh = new ComponentMesh();


				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				if (material) {
					compMat = new ComponentMaterial();
					compMat = ImportMaterial(material);
					if (compMat != nullptr) {
						nodeGO->AddComponent(compMat, MATERIAL);						
					}
					else {
						delete compMat;
					}
				}
								
				std::string meshName = nodeGO->name;
				
				compMesh = ImportMesh(mesh,meshName.c_str());
				if (compMesh&&compMat) {
					compMesh->SetMaterial(compMat);
				}
				if (compMesh != nullptr) {
					nodeGO->AddComponent(compMesh, MESH);
				}
				else {
					delete compMesh;
				}
				
				
			}
		}
	}
	else {
		savedMatrix = savedMatrix * node->mTransformation;
	}
	if (!nodeGO) { nodeGO = parent; }
	for (uint i = 0; i < node->mNumChildren; i++) // recursivity
	{
		GameObject* child = ImportNodeRecursive(node->mChildren[i], scene, nodeGO);

	}
	return nodeGO;
}

ComponentMaterial * SceneImporter::ImportMaterial(aiMaterial * material) // imports material to dds
{
	ComponentMaterial* mat = nullptr;

	aiString texturePath;
	aiReturn ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
	if (ret == aiReturn_SUCCESS) {
		bool error = false;
		std::string path;
		std::string textureName;
		std::string extension;
		App->fileSys->GetNameFromPath(texturePath.C_Str(), &path, &textureName, nullptr, nullptr);
		App->fileSys->GetNameFromPath(texturePath.C_Str(), nullptr, nullptr, nullptr, &extension);

		GLuint check = App->textures->CheckIfImageAlreadyLoaded(textureName.c_str());
		if (check == -1) {
			std::string texDDSPath;
			if (extension != DDS_FORMAT) {

				error = App->renderer3D->texImporter->ImportToDDS(texturePath.C_Str(), textureName.c_str());
				if (error) {
					std::string texAssetsPath = TEXTURES_PATH + textureName + extension; //THIS CANNOT LOAD SCENES THAT ARE NOT IN ASSETS/TEXTURE FOLDER
					error = App->renderer3D->texImporter->ImportToDDS(texAssetsPath.c_str(), textureName.c_str());
				}
				texDDSPath = LIB_TEXTURES_PATH + textureName + DDS_FORMAT;

			}
			else {
				texDDSPath = LIB_TEXTURES_PATH + textureName + DDS_FORMAT;
				if (path != LIB_TEXTURES_PATH) {
					std::string fllPath = TEXTURES_PATH + textureName + extension;
					App->fileSys->NormalizePath(fllPath);
					App->fileSys->Copy(fllPath.c_str(), texDDSPath.c_str());
				}

			}
			if (!error) {
				mat = new ComponentMaterial();
				mat->texture = new Material();
				
				mat->texture->textureID = App->renderer3D->texImporter->LoadTexture(texDDSPath.c_str(), mat->texture);
				OWN_LOG("Loading imported DDS texture from Lib/Textures folder");
				if (mat->texture->textureID == -1) { // first check if texture is in local path "Lib/Textures"
					OWN_LOG("Error loading texture.");
				}
				if (mat->texture->textureID != -1) { // if texture can be loaded
					//materialIDs[i] = mat->textureID;
					mat->texture->name = textureName;
					App->textures->AddMaterial(mat->texture);
					 
				}
			}
		}
		else {
			mat = new ComponentMaterial();
			mat->texture = App->textures->GetMaterialsFromID(check);
		}
	}
	else {

		OWN_LOG("Error loading texture from fbx. Error: %s", aiGetErrorString());
	}

	return mat;
}

ComponentMesh * SceneImporter::ImportMesh(aiMesh * mesh, const char* peiName)
{

	bool error = false;
	ComponentMesh* newMesh = new ComponentMesh();;
	newMesh->num_vertex = mesh->mNumVertices; //-----------vertex
	newMesh->vertex = new float3[newMesh->num_vertex];
	memcpy(newMesh->vertex, mesh->mVertices, sizeof(float3) * newMesh->num_vertex);
	OWN_LOG("Importing new mesh with %d vertices", newMesh->num_vertex);

	//texMeshIDs[meshNum] = new_mesh->mMaterialIndex;

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

	if (mesh->HasNormals() && !error) { //------------normals
		newMesh->num_normals = newMesh->num_vertex;
		newMesh->normals = new float3[newMesh->num_normals];
		memcpy(newMesh->normals, mesh->mNormals, sizeof(float3) * newMesh->num_normals);
	}
	if (mesh->GetNumUVChannels() > 0 && !error) { //------------textureCoords
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
	else{

		std::string fileName = LIB_MODELS_PATH;
		fileName += peiName;
		fileName += OWN_FILE_FORMAT;
		std::ofstream dataFile(fileName.c_str(), std::fstream::out | std::fstream::binary);
		OWN_LOG("Iporting file to PEI");

		uint ranges[4] = { newMesh->num_index,newMesh->num_vertex, newMesh->num_normals, newMesh->num_textureCoords };

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
	
		newMesh->GenerateBuffer();
		
		dataFile.close();
		RELEASE_ARRAY(data);
		cursor = nullptr;
	}
	return newMesh;
}


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

void SceneImporter::CleanUp()
{
	aiDetachAllLogStreams();
}


float4x4 SceneImporter::aiMatrixToFloat4x4(aiMatrix4x4 matrix)
{
	float mat[16] =
	{
		matrix.a1, matrix.a2, matrix.a3, matrix.a4,
		matrix.b1, matrix.b2, matrix.b3, matrix.b4,
		matrix.c1, matrix.c2, matrix.c3, matrix.c4,
		matrix.d1, matrix.d2, matrix.d3, matrix.d4
	};

	float4x4 nwMatrix;
	nwMatrix.Set(mat);
	return nwMatrix;
}