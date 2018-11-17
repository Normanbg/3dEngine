#include "Application.h"
#include "ModuleResources.h"
#include "ResourceAudio.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ModuleScene.h"
#include "Timer.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"


ModuleResources::ModuleResources() 
{
}


ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init(JSON_Object * obj)
{
	

	return true;
}

bool ModuleResources::Start()
{
	CheckMetaFiles();
	return true;
}

update_status ModuleResources::PreUpdate(float dt)
{
	if (!App->scene->inGame) {
		if (lastCheck.Read() > MS_TO_CHECK_META) {
			lastCheck.SetZero();
			CheckMetaFiles();
		}
	}
	else {
		lastCheck.Stop();
	}
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	return true;
}

void ModuleResources::CheckMetaFiles()
{
	
	std::vector<std::string> files;
	std::vector<std::string> dirs;	
	
	App->fileSys->GetFilesFromDir(ASSETS_PATH, files, dirs, true);

	std::map<std::string, uint> missingComplementary; // stores resource/meta to find if all resouces has .meta
	
	int i = 0;
	for (std::vector<std::string>::const_iterator fileIt = files.begin() ; fileIt != files.end(); fileIt++, i++) {

		bool meta = App->fileSys->IsMetaFile((*fileIt).c_str());
		std::string fileName;
		meta ? App->fileSys->GetNameFromPath((*fileIt).c_str(), nullptr, &fileName, nullptr, nullptr)
			: App->fileSys->GetNameFromPath((*fileIt).c_str(), nullptr, nullptr, &fileName, nullptr);
		
		bool hasComplementary = false;
		for (std::map<std::string, uint>::iterator missFileIt = missingComplementary.begin(); missFileIt != missingComplementary.end(); missFileIt++) {
			if (fileName == (*missFileIt).first) 
			{
				if (ManageResourceWithMeta(files[(*missFileIt).second].c_str(), (*fileIt).c_str())==false) {
					ImportFileAndGenerateMeta(files[(*missFileIt).second].c_str());
				}
				missingComplementary.erase(missFileIt);
				hasComplementary = true;// meta/resource finds its resource/meta
				
				//check timestamp

				break;
			}
		}
		if (!hasComplementary) { // meta/resources that doesn't have resource/meta
			missingComplementary[fileName] = i;
		}
	}
	//----Meta / Resources Unmatched stored in metaChecker together 

	if (missingComplementary.size() > 0) {
		for (std::map<std::string, uint>::iterator fileIt = missingComplementary.begin(); fileIt != missingComplementary.end(); fileIt++) {
		
			bool meta = App->fileSys->IsMetaFile(files[(*fileIt).second]);
			if (meta) {
				OWN_LOG("resource missing! %s", files[(*fileIt).second].c_str());
				RemoveResource(files[(*fileIt).second].c_str());
					
			}
			else {
				OWN_LOG("meta missing! %s", files[(*fileIt).second].c_str())
					ImportFileAndGenerateMeta(files[(*fileIt).second].c_str());
			}
		}
	}
	lastCheck.Start();
}

uuid ModuleResources::FindByPath(const char * fileInAssets, Resource::ResType type) const
{
	std::string fileName = fileInAssets;
	
	for (std::map<uuid, Resource*>::const_iterator it = resources.begin(); it != resources.end(); it++) {
		if (it->second->GetPathStr() == fileName) {
			if(type == Resource::ResType::None || type == it->second->GetType())
			return it->first;
		}	
	}
	return 0;
}

uuid ModuleResources::FindByExportedFile(const char * name, Resource::ResType type) const
{
	std::string fileName = name;

	for (std::map<uuid, Resource*>::const_iterator it = resources.begin(); it != resources.end(); it++) {
		if (it->second->GetExportedFile() == fileName) {
			if (type == Resource::ResType::None || type == it->second->GetType())
				return it->first;
		}
	}
	return 0;
}

uuid ModuleResources::FindByName(const char * fileInAssets, Resource::ResType type) const
{
	if (fileInAssets != nullptr) {
		std::string fileName = fileInAssets;

		for (std::map<uuid, Resource*>::const_iterator it = resources.begin(); it != resources.end(); it++) {
			if (it->second->GetNameStr() == fileName) {
				if (type == Resource::ResType::None || type == it->second->GetType())
					return it->first;
			}
		}
	}
	return 0;
}

bool ModuleResources::ImportFileAndGenerateMeta(const char * newFileInAssets)
{
	bool ret = false;
	bool import_ok = false; 
	std::vector<std::string> exportedFiles;
	Resource::ResType type = GetResourceTypeFromExtension(newFileInAssets);
	uuid resourceUUID = App->scene->GetRandomUUID();
	switch (type) {
	case Resource::ResType::Texture: {import_ok = App->texImporter->ImportTexture(newFileInAssets, &exportedFiles); break; }
	case Resource::ResType::Scene: {import_ok = App->importer->ImportScene(newFileInAssets, &exportedFiles, resourceUUID); break; }
	case Resource::ResType::Mesh: {import_ok = App->fileSys->CopyPEItoLib(newFileInAssets, &exportedFiles, resourceUUID); break; }
	case Resource::ResType::Audio: {import_ok = App->fileSys->CopyAudioToLib(newFileInAssets, &exportedFiles); break; }
	case Resource::ResType::None: {return false; }
	}
	if (import_ok) {
		std::vector<uuid> uuids;
		for (int i = 0; i < exportedFiles.size(); i++) {
			
			Resource* res = CreateNewResource(type);			
			
			res->SetPath(newFileInAssets);
			res->SetExportedFile(exportedFiles[i].c_str());

			std::string name;
			App->fileSys->GetNameFromPath(exportedFiles[i].c_str(), nullptr, &name, nullptr, nullptr);
			
			if (type!=Resource::ResType::Scene){// if is scene, scene.meta wil store uuid of its meshes in "uuids" and the uuid of the resourceScene in "resourceUUID"
				
				res->SetName(name.c_str());
				res->SetUUID(resourceUUID);
			} 
			else{
				std::string  pei;
				App->fileSys->GetUUID_PEI(name.c_str(), nullptr, &pei);
				res->SetName(pei.c_str());
				uuids.push_back(res->GetUUID());
			}
		}		
			
		GenerateMetaFile(newFileInAssets, resourceUUID, uuids );
		ret = true;
	}
	return ret;
}


const Resource * ModuleResources::Get(uuid _uuid) const
{
	for (std::map<uuid, Resource*>::const_iterator it = resources.begin(); it != resources.end(); it++) {
		if (it->first == _uuid) {
			return it->second;
		}
	}
	return nullptr;
}

Resource * ModuleResources::Get(uuid _uuid)
{
	for (std::map<uuid, Resource*>::iterator it = resources.begin(); it != resources.end(); it++) {
		if (it->first == _uuid) {
			return it->second;
		}
	}
	return nullptr;
}

void ModuleResources::LoadFiles(const char * filePath)
{
	Resource::ResType type = GetResourceTypeFromExtension(filePath);
	
	switch (type) {
	case Resource::ResType::Texture: {
		std::string tName;
			App->fileSys->GetNameFromPath(filePath, nullptr, &tName, nullptr, nullptr);
		GameObject* GO;
		GO = App->scene->AddGameObject(tName.c_str());
		ComponentMaterial* mat = (ComponentMaterial*)GO->AddComponent(MATERIAL);
		mat->SetResource(App->resources->FindByPath(filePath)); break; }
	case Resource::ResType::Scene: {App->importer->LoadFBXScene(filePath); break; }
	}

}

std::vector<Resource*> ModuleResources::GetResourcesListType(Resource::ResType type, bool loaded) 
{
	std::vector<Resource*> ret;
	for (std::map<uuid, Resource*>::iterator it = resources.begin(); it != resources.end(); it++) {
		if (it->second->GetType() == type) {
			if (loaded) {
				if (it->second->IsLoaded()) {
					ret.push_back(it->second);
				}
			}
			else
				ret.push_back(it->second);
		}
	}
	return ret;
}

Resource * ModuleResources::CreateNewResource(Resource::ResType type, uuid forceUUID)
{
	Resource* res = nullptr;
	uuid uuid;

	if (forceUUID != 0 && Get(forceUUID) == nullptr)
		uuid = forceUUID;
	else
		uuid = App->scene->GetRandomUUID();
	if (Get(uuid) != nullptr) {
		OWN_LOG("Resource already created! UUID matching");
		return nullptr;
	}

	switch (type) {
	case Resource::ResType::Texture: {	res = (Resource*) new ResourceTexture(uuid); break;	}
	case Resource::ResType::Mesh: {	res = (Resource*) new ResourceMesh(uuid); break;	}
	case Resource::ResType::Audio: {res = (Resource*) new ResourceAudio(uuid); break;	}
	case Resource::ResType::Scene: {res = (Resource*) new ResourceMesh(uuid); break;	}
	}
	if (res != nullptr) {
		resources[uuid] = res;
	}
	return res;
}

void ModuleResources::RemoveResource(const char * metaPath)
{
	
	std::string mPath = metaPath;
	char* buffer = nullptr;
	App->fileSys->readFile(mPath.c_str(), &buffer);
	Config meta(buffer);
	uuid UUID = meta.GetUInt("ResourceUUID", 0);
	uint childs = meta.GetUInt("UUID Childs Num", 0);
	Resource* res= nullptr;
	if (childs > 0) {
		for (int i = 0; i < childs; i++) {
			std::string uuidNum = "UUID_" + std::to_string(i);
			uint childUUID = meta.GetUInt(uuidNum.c_str(), 0);
			res = Get(childUUID);
			res->CleanUp();
			resources.erase(res->GetUUID());
			App->fileSys->RemoveFile(res->GetExportedFile().c_str());
		}
	}
	else {		
		res = Get(UUID);
		resources.erase(res->GetUUID());
		res->CleanUp();
		App->fileSys->RemoveFile(res->GetExportedFile().c_str());
	}

	App->fileSys->RemoveFile(metaPath);
	res = nullptr;
}

void ModuleResources::RemoveResource(uuid UUID)
{
	resources.erase(UUID);
}

void ModuleResources::GenerateMetaFile(const char* assetFile,  uuid resourceUUID, std::vector<uuid> exportedUUIDs )
{
	Config meta;
	uint timestamp = App->fileSys->GetLastModification(assetFile);
	Resource::ResType type = GetResourceTypeFromExtension(assetFile);
	meta.AddUInt("TimeStamp", timestamp);	
	meta.AddUInt("ResourceUUID", resourceUUID);	
	Resource* res = Get(resourceUUID);
	if (type == Resource::ResType::Scene) {
		meta.AddUInt("UUID Childs Num", exportedUUIDs.size());
		for (int i = 0; i < exportedUUIDs.size(); i++) {
			std::string uuidNum = "UUID_" + std::to_string(i);
			meta.AddUInt(uuidNum.c_str(), exportedUUIDs[i]);
			std::string uuidPeiNum = "UUID_PEIName_" + std::to_string(i);					
			meta.AddString(uuidPeiNum.c_str(), Get(exportedUUIDs[i])->GetName());
			Resource* res = Get(exportedUUIDs[i]);
			res->Save(meta);
		}
	}
	else {
		res->Save(meta);
	}

	char* buffer = nullptr;
	uint size = meta.Save(&buffer);
	std::string name = assetFile;
	//App->fileSys->GetNameFromPath(assetFile, nullptr, &name, nullptr, nullptr);	
	name += META_FORMAT;
	App->fileSys->writeFile(name.c_str(), buffer, size);
	RELEASE_ARRAY(buffer);
}

const Resource::ResType ModuleResources::GetResourceTypeFromExtension(const char * path) const
{	
	std::string extension;
	App->fileSys->GetNameFromPath(path, nullptr, nullptr, nullptr, &extension);
	if (extension == OWN_FILE_FORMAT || extension == OWN_FILE_FORMAT_CAP)  return Resource::ResType::Mesh;
	if (extension == FBX_FORMAT || extension == FBX_FORMAT_CAP)  return Resource::ResType::Scene;
	if (extension == DDS_FORMAT || extension == DDS_FORMAT_CAP)  return Resource::ResType::Texture;
	if (extension == PNG_FORMAT || extension == PNG_FORMAT_CAP)  return Resource::ResType::Texture;
	if (extension == JPG_FORMAT || extension == JPG_FORMAT_CAP)  return Resource::ResType::Texture;
	if (extension == JPEG_FORMAT || extension == JPEG_FORMAT_CAP)  return Resource::ResType::Texture;
	if (extension == TGA_FORMAT || extension == TGA_FORMAT_CAP)  return Resource::ResType::Texture;
	if (extension == WAV_FORMAT || extension == WAV_FORMAT_CAP)  return Resource::ResType::Audio;
	if (extension == OGG_FORMAT || extension == OGG_FORMAT_CAP)  return Resource::ResType::Audio;
	
	return Resource::ResType::None;
}

void ModuleResources::ChangeResourceUUID(uuid formerUUID, uuid newUUID)
{
	for (std::map<uuid, Resource*>::iterator it = resources.begin(); it != resources.end(); it++) {
		if (it->first == formerUUID) {
			resources[newUUID] = resources[formerUUID];
			resources.erase(formerUUID);
			return;
		}
	}
}

bool ModuleResources::ManageResourceWithMeta(const char * resource, const char * metaPath) 
{
	uint resourceLastMod = App->fileSys->GetLastModification(resource);
	char* buffer = nullptr;
	uint size = App->fileSys->readFile(metaPath, &buffer);
	if (size < 0) {
		OWN_LOG("Error loading meta %s. All data not loaded.", metaPath)
			RELEASE_ARRAY(buffer);
		return false;
	}
	Config meta(buffer);

	uint lastMetaMod = meta.GetUInt("TimeStamp", 0);
	if (lastMetaMod != resourceLastMod) { // file is modified
		RELEASE_ARRAY(buffer);
		return false;
	}



	
	uuid resourceUUID = meta.GetUInt("ResourceUUID", 0);
	if (Get(resourceUUID)) { // if texture or mesh is already in resources
		return true;
	}
	Resource::ResType type = GetResourceTypeFromExtension(resource);
	if (type == Resource::ResType::Scene) {
		uint childs = meta.GetUInt("UUID Childs Num", 0);
	for (int i = 0; i < childs; i++) {

			std::string uuidNum = "UUID_" + std::to_string(i);
			uint childUUID = meta.GetUInt(uuidNum.c_str(), 0);

			if (Get(childUUID)) { // if scene mesh is already in resources
				return true;
			}

			std::string uuidPeiNum = "UUID_PEIName_" + std::to_string(i);
			std::string uuidPei = meta.GetString(uuidPeiNum.c_str(), "");

			std::string pei;
			App->fileSys->GetUUID_PEI(uuidPei.c_str(), nullptr, &pei);			
			
			Resource* res = CreateNewResource(Resource::ResType::Mesh, childUUID);			
			
			res->SetName(pei.c_str());
			res->SetPath(resource);

			std::string exportedFile;
			exportedFile = LIB_MODELS_PATH + std::to_string(resourceUUID)+ "~" + pei + OWN_FILE_FORMAT;
			res->SetExportedFile(exportedFile);
			res->Load(meta);
		}
	}
	else {
		Resource* res = CreateNewResource(type, resourceUUID);
		std::string name;
		App->fileSys->GetNameFromPath(resource, nullptr, &name, nullptr, nullptr);
		res->SetName(name.c_str());
		res->SetPath(resource);
		res->Load(meta);
	}
	RELEASE_ARRAY(buffer);
	return true;
}
