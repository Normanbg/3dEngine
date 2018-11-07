

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

uint* SceneImporter::ImportFBXtoPEI(const char * FBXpath)
{
	std::string fullFBXPath = MODELS_PATH;
	fullFBXPath+= FBXpath;
		
	std::string modelName;
	
	App->fileSys->GetNameFromPath(fullFBXPath.c_str(),nullptr, &modelName, nullptr,nullptr);

	const aiScene* scene = aiImportFile(fullFBXPath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality); 

	if (scene == nullptr) {		
		 scene = aiImportFile(FBXpath, aiProcessPreset_TargetRealtime_MaxQuality);
		 if (scene == nullptr) {
			 OWN_LOG("Error loading fbx from Assets/3DModels folder.");
			 aiReleaseImport(scene);
			 
			 return nullptr;
		 }	
	}



	uint numMaterials = scene->mNumMaterials;
	uint* materialIDs = new uint[scene->mNumMaterials];

	if (scene->HasMaterials()) {
		//need to load a texture outside the mesh. as a scene
		///check if material is in the path
		///if it is, load texture, convert to .dds & added as a component
		
		

		for (int i = 0; i < numMaterials; i++) {

			aiMaterial* material = scene->mMaterials[i];

			Material* mat = new Material;

			aiColor3D color(0.f, 0.f, 0.f);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			mat->colors[0] = color.r;
			mat->colors[1] = color.g;
			mat->colors[2] = color.b;

			materialIDs[i] = -1; // set initially to -1 to avoid errors;

			aiString texturePath;
			aiReturn ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
			if (ret == aiReturn_SUCCESS) {
				bool error= false;
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
						mat->textureID = App->renderer3D->texImporter->LoadTexture(texDDSPath.c_str(), mat);
						OWN_LOG("Loading imported DDS texture from Lib/Textures folder");
						if (mat->textureID == -1) { // first check if texture is in local path "Lib/Textures"
							OWN_LOG("Error loading texture.");
						}
						if (mat->textureID != -1) { // if texture can be loaded
							materialIDs[i] = mat->textureID;
							mat->name = textureName;
							App->textures->AddMaterial(mat);
						}
					}
				}	
				else {
					materialIDs[i] = check;
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

		newScene.numMeshes = scene->mNumMeshes;
		uint* meshIDs = new uint[newScene.numMeshes];

		const aiNode* node = scene->mRootNode; // NEEDTO delete pointer?
		
		aiVector3D _scale;
		aiQuaternion _rotation;
		aiVector3D _position;
		node->mTransformation.Decompose(_scale, _rotation, _position);
		newScene.scale = { _scale.x, _scale.y, _scale.z };
		newScene.position = { _position.x, _position.y, _position.z };
		newScene.rotation = { _rotation.x, _rotation.y, _rotation.z, _rotation.w };

		
		

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

		for (int i = 0; i < scene->mNumMeshes; i++) {
			meshIterator = scene->mMeshes[i];
			ImportFromMesh(scene, meshIterator, &dataFile,i, materialIDs, meshIDs);
		}

		uint* texMeshIDs = new uint[scene->mNumMeshes];
		for (int i = 0; i < scene->mNumMeshes; i++) {
			texMeshIDs[i] = materialIDs[meshIDs[i]];

		}
		
		
		delete[] materialIDs;
		materialIDs = nullptr;
		delete[] meshIDs;
		meshIDs = nullptr;
		aiReleaseImport(scene);		
		dataFile.close();

		return texMeshIDs;
	}
	else {
		OWN_LOG("Error loading FBX, scene has no meshes");
	}
	
	return nullptr;

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
		
		std::string fileName = LIB_MODELS_PATH + modelName + OWN_FILE_FORMAT;
		std::ofstream dataFile(fileName.c_str(), std::fstream::out | std::fstream::binary);
		OWN_LOG("Creating PEI file");

		uint size = sizeof(uint);
		char* data = new char[size];
		memcpy(data, &scene->mNumMeshes, size);

		dataFile.write(data, size);
		
		GameObject* GOchild = ImportNodeRecursive(scene->mRootNode, scene, GO, &dataFile);
		
		RELEASE_ARRAY(data);
		GOchild = nullptr;
		GO = nullptr;
		aiReleaseImport(scene);
		dataFile.close();
	}
}

GameObject * SceneImporter::ImportNodeRecursive(aiNode * node, const aiScene * scene, GameObject * parent, std::ofstream* dataFile)
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
				

				uint size = sizeof(float3) * 2 + sizeof(Quat); // numMeshes+(scale+pos) + rotation in bytes
				char* data = new char[size];
				char* cursor = data;


				uint bytes = sizeof(float3);//sizeof float3		
				memcpy(cursor, &nodeGO->transformComp->getScale(), bytes);

				cursor += bytes;//sizeof float3		
				memcpy(cursor, &nodeGO->transformComp->getPos(), bytes);

				cursor += bytes;
				bytes = sizeof(Quat);//sizeof float3
				memcpy(cursor, &nodeGO->transformComp->getQuatRot(), bytes);

				dataFile->write(data, size);

				RELEASE_ARRAY(data);
				cursor = nullptr;

				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

				ComponentMesh* compMesh = new ComponentMesh();
				compMesh = ImportMesh(mesh, dataFile);
				if (compMesh != nullptr) {
					nodeGO->AddComponent(compMesh, MESH);
				}
				else {
					delete compMesh;
				}
				
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
								
				if (material) {
					ComponentMaterial* compMat = new ComponentMaterial(); 
					compMat = ImportMaterial(material);
					if (compMat!=nullptr) {
						
						nodeGO->AddComponent(compMat, MATERIAL);
						if (compMesh) { compMesh->SetMaterial(compMat); }
					}
					else {
						delete compMat;
					}
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
		GameObject* child = ImportNodeRecursive(node->mChildren[i], scene, nodeGO, dataFile);

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

ComponentMesh * SceneImporter::ImportMesh(aiMesh * mesh, std::ofstream* dataFile)
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

		dataFile->write(data, size);
	
		newMesh->GenerateBuffer();

		RELEASE_ARRAY(data);
		cursor = nullptr;
	}
	return newMesh;
}


void SceneImporter::ImportFBXandLoad(const char * fbxPath)
{
	uint* texMeshLinker = ImportFBXtoPEI(fbxPath);

	std::string PEIPath;
	App->fileSys->GetNameFromPath(fbxPath, nullptr, &PEIPath, nullptr,nullptr);
	PEIPath += OWN_FILE_FORMAT;
	
	LoadPEI(PEIPath.c_str(),texMeshLinker);
	delete[] texMeshLinker;
	texMeshLinker = nullptr;
}


void SceneImporter::ImportFromMesh(const aiScene* currSc, aiMesh * new_mesh,std::ofstream* dataFile, uint meshNum,  uint* texIDs, uint* texMeshIDs)
{
	bool error = false;
	

	SceneImporter::dataMesh newMesh;
	newMesh.num_vertex = new_mesh->mNumVertices; //-----------vertex
	newMesh.vertex = new float3[newMesh.num_vertex];
	memcpy(newMesh.vertex, new_mesh->mVertices, sizeof(float3) * newMesh.num_vertex);
	OWN_LOG("Importing new mesh with %d vertices", newMesh.num_vertex);
	
	texMeshIDs[meshNum] = new_mesh->mMaterialIndex;
	
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
		newMesh.num_normals = newMesh.num_vertex;
		newMesh.normals = new float3[newMesh.num_normals];
		memcpy(newMesh.normals, new_mesh->mNormals, sizeof(float3) * newMesh.num_normals);
	}
	if (new_mesh->GetNumUVChannels() > 0 && !error) { //------------textureCoords
		newMesh.num_texCoords = newMesh.num_vertex;
		newMesh.texturesCoords = new float2[newMesh.num_texCoords];

		for (int i = 0; i < (newMesh.num_texCoords); i++) {
			newMesh.texturesCoords[i].x = new_mesh->mTextureCoords[0][i].x;
			newMesh.texturesCoords[i].y = new_mesh->mTextureCoords[0][i].y;
		}
	}
			
	if (!error) { // writting into file

		uint ranges[4] = { newMesh.num_index, newMesh.num_vertex, newMesh.num_normals, newMesh.num_texCoords };

		uint size = sizeof(ranges) + sizeof(uint)*newMesh.num_index + sizeof(float3)*newMesh.num_vertex + sizeof(float3)*newMesh.num_normals + sizeof(float2) * newMesh.num_texCoords; // numIndex + numVertex + index + vertex + normals + textureCoords

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
		bytes = sizeof(float3)* newMesh.num_normals;
		memcpy(cursor, newMesh.normals, bytes);
		
		cursor += bytes;		
		bytes = sizeof(float2)* newMesh.num_texCoords;
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

	if (error) {
		newMesh.num_index = newMesh.num_vertex = newMesh.num_normals = newMesh.num_texCoords = 0;
		uint ranges[4] = { newMesh.num_index, newMesh.num_vertex, newMesh.num_normals, newMesh.num_texCoords };
		uint size = sizeof(ranges) + sizeof(uint)*newMesh.num_index + sizeof(float3)*newMesh.num_vertex + sizeof(float3)*newMesh.num_normals + sizeof(float2) * newMesh.num_texCoords; // numIndex + numVertex + index + vertex + normals + textureCoords
		char* data = new char[size];
		uint bytes = sizeof(ranges);
		memcpy(data, ranges, bytes);
		dataFile->write(data, size);
		delete[] data;
		data = nullptr;
	}
	
}

void SceneImporter::newLoadPEI(const char * fileName) {

	std::string modelName;

	App->fileSys->GetNameFromPath(fileName, nullptr, &modelName, nullptr, nullptr);

	Timer loadingTimer;

	GameObject* gameObject = App->scene->AddGameObject(modelName.c_str());


	ComponentTransformation* compTrans = gameObject->GetTransformComponent();

	std::string filePath = LIB_MODELS_PATH;
	filePath += modelName + OWN_FILE_FORMAT;

	

	std::ifstream dataFile(filePath.c_str(), std::fstream::out | std::fstream::binary);
	if (dataFile.fail()) {
		OWN_LOG("Error loading PEI. Cannot find PEI file %s in Lib/Textures", fileName);
		return;
	}

	uint size = sizeof(uint);
	char* scenedata = new char[size];	
	dataFile.read(scenedata, size);

	uint numMeshes = 0;
	uint bytes = sizeof(size);
	memcpy(&numMeshes, scenedata, bytes);

	RELEASE_ARRAY(scenedata);
	
	uint totalSize = 0;
	totalSize = size;

	for (int i = 0; i < numMeshes; i++) {
		std::string childName = modelName + "_mesh" + std::to_string(i);

		GameObject* childGO = new GameObject(childName.c_str());
		gameObject->AddChildren(childGO);

		

		ComponentTransformation* compTrans = childGO->transformComp;

		dataFile.seekg(totalSize);
		

		uint ranges[4] = { 0,0,0,0 }; // [numIndex , numVertex, numNormals, numTexCoords]

		uint headerSize = sizeof(float3)*2 + sizeof(Quat) + sizeof(ranges);

		char* headerdata = new char[headerSize];
		char* cursor = headerdata;		

		dataFile.read(headerdata, headerSize);
		float3 sc = float3::zero;
		float3 pos = float3::zero;
		Quat qua = { 0,0,0,0 };

		bytes = sizeof(float3);
		memcpy(&sc, cursor, bytes);
		compTrans->setScale(sc);

		cursor += bytes;
		memcpy(&pos, cursor, bytes);
		compTrans->setPos(pos);

		cursor += bytes;
		bytes = sizeof(Quat);//sizeof quat
		memcpy(&qua, cursor, bytes);
		compTrans->setRotQuat(qua);
				
		cursor += bytes;
		bytes = sizeof(ranges);
		memcpy(ranges, cursor, bytes);

		ComponentMesh* compMesh = (ComponentMesh*)childGO->AddComponent(MESH);

		//---

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


		dataFile.seekg(totalSize + headerSize);

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

		totalSize += headerSize + meshDataSize;

		compMesh->GenerateBuffer();


		RELEASE_ARRAY(headerdata);
		RELEASE_ARRAY(meshdata);
		mcursor = nullptr;
		compMesh = nullptr;

	}
	OWN_LOG("Succesfully loaded PEI: %s. Loading time: %i ms \n", modelName.c_str(), loadingTimer.Read());
	gameObject = nullptr;

	dataFile.close();
}

void SceneImporter::LoadPEI(const char * fileName, uint* meshTexLinker)
{
	
	
	std::string modelName;

	App->fileSys->GetNameFromPath(fileName, nullptr, &modelName, nullptr, nullptr);

	if (App->renderer3D->GetLoadFBXTest()) { // loads .fbx to test the ms compared to loading .pei
		std::string fbxName = modelName;
		fbxName+= FBX_FORMAT;
		LoadFBX(fbxName.c_str());
	}
	Timer loadingTimer;

	GameObject* gameObject = App->scene->AddGameObject(modelName.c_str());
	
	
	ComponentTransformation* compTrans = gameObject->GetTransformComponent();
	
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
	memcpy(&compTrans->getScale(), cursor, bytes);

	cursor += bytes;
	
	memcpy(&compTrans->getPos(), cursor, bytes);

	cursor += bytes;
	bytes = sizeof(Quat);//sizeof quat
	memcpy(&compTrans->getQuatRot(), cursor, bytes);
	
	RELEASE_ARRAY(scenedata);
	cursor = nullptr;

	

	uint totalSize = 0;
	totalSize = size;

	for (int i = 0; i < numMeshes; i++) {
		std::string childName = modelName + "_mesh"+std::to_string(i);
		
		GameObject* childGO = new GameObject(childName.c_str());
		gameObject->AddChildren(childGO);

		bool texture = false;
		ComponentMaterial* compMat;
		if (meshTexLinker[i] != -1) {

			compMat = (ComponentMaterial*)childGO->AddComponent(MATERIAL);
			compMat->texture = App->textures->GetMaterialsFromID(meshTexLinker[i]);
			
			texture = true;
		}
		ComponentMesh* compMesh = (ComponentMesh*)childGO->AddComponent(MESH);
		if (texture) {
			compMesh->SetMaterial(compMat);
		}
		
		
			 
		
				
		//---read ranges
		uint ranges[4]; // [numIndex , numVertex, numNormals, numTexCoords]

		uint rangesSize = sizeof(ranges); 

		

		char* headerdata = new char[rangesSize];		

		dataFile.seekg(totalSize);

		dataFile.read(headerdata, rangesSize);

		uint mbytes = sizeof(ranges);
		memcpy(ranges, headerdata, mbytes); 

		
		
		//---

		compMesh->num_index = ranges[0];
		compMesh->num_vertex = ranges[1];
		compMesh->num_normals = ranges[2];
		compMesh->num_textureCoords = ranges[3];
	
		compMesh->index = new uint[compMesh->num_index];
		compMesh->vertex = new float3[compMesh->num_vertex];
		compMesh->normals = new float3[compMesh->num_normals];
		compMesh->texturesCoords = new float2[compMesh->num_textureCoords];

		uint meshDataSize = sizeof(uint)* compMesh->num_index + sizeof(float3)*compMesh->num_vertex+ sizeof(float3)*compMesh->num_normals + sizeof(float2) * compMesh->num_textureCoords;

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
		mbytes = sizeof(float3)*compMesh->num_normals;//normals	
		memcpy(compMesh->normals, mcursor, mbytes);

		mcursor += mbytes;
		mbytes = sizeof(float2)*compMesh->num_textureCoords; //texCoords
		memcpy(compMesh->texturesCoords,mcursor, mbytes);

		totalSize += rangesSize + meshDataSize;

		compMesh->GenerateBuffer();
		
		
		RELEASE_ARRAY(headerdata);		
		RELEASE_ARRAY(meshdata);
		mcursor = nullptr;
		compMesh = nullptr;
		
	}
	OWN_LOG("Succesfully loaded PEI: %s. Loading time: %i ms \n", modelName.c_str(), loadingTimer.Read());
	gameObject = nullptr;

	dataFile.close();
}

bool SceneImporter::LoadMeshPEI(ComponentMesh * mesh)
{
	
	std::string meshName;
	uint numMesh = 0;

	App->fileSys->GetNameFromMesh(mesh->myGO->name.c_str(), &meshName, numMesh);
	if (numMesh != -1) {
		uint totalSize = 0;
		std::string filePath = LIB_MODELS_PATH;
		filePath += meshName + OWN_FILE_FORMAT;

		

		std::ifstream dataFile(filePath.c_str(), std::fstream::out | std::fstream::binary);
		if (dataFile.fail()) {
			OWN_LOG("Error loading PEI. Cannot find PEI file %s in Library", meshName.c_str());
			return false;
		}

		uint size = sizeof(uint) + sizeof(float3) * 2 + sizeof(Quat);
		
		char* scenedata = new char[sizeof(uint)];
	
		dataFile.read(scenedata, sizeof(uint));

		uint numMeshes = 0;
		uint bytes = sizeof(uint);
		memcpy(&numMeshes, scenedata, bytes);

		RELEASE_ARRAY(scenedata);
		uint headerSize = sizeof(uint) + sizeof(float3) * 2 + sizeof(Quat);
		totalSize += headerSize;
		for (int i = 0; i < numMeshes; i++) {
			uint ranges[4]; // [numIndex , numVertex, numNormals, numTexCoords]

			uint rangesSize = sizeof(ranges);

			char* rangesdata = new char[rangesSize];
			dataFile.seekg(totalSize);
			dataFile.read(rangesdata, rangesSize);

			uint mbytes = sizeof(ranges);
			memcpy(ranges, rangesdata, mbytes);

			

			uint meshDataSize = sizeof(uint)* ranges[0] + sizeof(float3)*ranges[1] + sizeof(float3)* ranges[2] + sizeof(float2) *  ranges[3];

			if (i == numMesh) {
				mesh->num_index = ranges[0];
				mesh->num_vertex = ranges[1];
				mesh->num_normals = ranges[2];
				mesh->num_textureCoords = ranges[3];

				mesh->index = new uint[mesh->num_index];
				mesh->vertex = new float3[mesh->num_vertex];
				mesh->normals = new float3[mesh->num_normals];
				mesh->texturesCoords = new float2[mesh->num_textureCoords];

				

				char* meshdata = new char[meshDataSize];
				char* mcursor = meshdata;


				dataFile.seekg(totalSize + rangesSize);

				dataFile.read(meshdata, meshDataSize);


				mbytes = sizeof(uint) *mesh->num_index;//index		
				memcpy(mesh->index, mcursor, mbytes);

				mcursor += mbytes;
				mbytes = sizeof(float3)*mesh->num_vertex;//vertex		
				memcpy(mesh->vertex, mcursor, mbytes);

				mcursor += mbytes;
				mbytes = sizeof(float3)*mesh->num_normals;//normals	
				memcpy(mesh->normals, mcursor, mbytes);

				mcursor += mbytes;
				mbytes = sizeof(float2)*mesh->num_textureCoords; //texCoords
				memcpy(mesh->texturesCoords, mcursor, mbytes);

				mesh->GenerateBuffer();
				dataFile.close();
				RELEASE_ARRAY(rangesdata);
				RELEASE_ARRAY(meshdata);
				mcursor = nullptr;
				return true;
			}
			totalSize += rangesSize + meshDataSize;
			RELEASE_ARRAY(rangesdata);

		}

		dataFile.close();

	}

	return false;
}

void SceneImporter::CleanUp()
{
	aiDetachAllLogStreams();
}

void SceneImporter::LoadFBX(const char * path) {

	Timer timerLoader;
	std::string fbxName = MODELS_PATH;
	fbxName += path;
	const aiScene* scene = aiImportFile(fbxName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	//---checks if fbx path exist
	if (scene == nullptr) {
		OWN_LOG("Error loading scene %s", aiGetErrorString());
		aiReleaseImport(scene);
		return;

	}//------

	std::string modelName;
	App->fileSys->GetNameFromPath(path, nullptr, &modelName,nullptr ,nullptr);

	GameObject* gameObject = App->scene->AddGameObject(modelName.c_str());

	
	ComponentTransformation* compTrans = gameObject->GetTransformComponent();

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
				App->fileSys->GetNameFromPath(fullTexPath.c_str(), nullptr, &textureName, nullptr, &extension);

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
						mat->name = textureName;
						//mesh.texID = _text.textureID;

						App->textures->AddMaterial(mat);
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
		OWN_LOG("Loading FBX mesh from %s", path);
		bool error = false;

		const aiNode* node = scene->mRootNode; // NEEDTO delete pointer?

		aiVector3D _scale;
		aiQuaternion _rotation;
		aiVector3D _position;
		node->mTransformation.Decompose(_scale, _rotation, _position);
		compTrans->setScale({ _scale.x, _scale.y, _scale.z });
		compTrans->setPos({ _position.x, _position.y, _position.z });
		compTrans->setRotQuat({ _rotation.x, _rotation.y, _rotation.z, _rotation.w });

		aiMesh * meshIterator = nullptr;
		for (int i = 0; i < scene->mNumMeshes; i++) {
			meshIterator = scene->mMeshes[i];

			std::string childName = modelName + "_mesh" + std::to_string(i);

			GameObject* childGO = new GameObject(childName.c_str());
			gameObject->AddChildren(childGO);

			ComponentMesh* compMesh = (ComponentMesh*)childGO->AddComponent(MESH);

			compMesh->num_vertex = meshIterator->mNumVertices;
			compMesh->vertex = new float3[compMesh->num_vertex];
			memcpy(compMesh->vertex, meshIterator->mVertices, sizeof(float3) * compMesh->num_vertex);
			OWN_LOG("New mesh with %d vertices", compMesh->num_vertex);

			if (meshIterator->HasFaces()) {
				compMesh->num_faces = meshIterator->mNumFaces ;
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
		OWN_LOG("Succesfully loaded FBX: %s. Loading time: %i ms \n", modelName.c_str(), timerLoader.Read());
	}
	else {
		OWN_LOG("Error loading FBX, scene has no meshes");
	}
	aiReleaseImport(scene);
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
