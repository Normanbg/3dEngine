#include "Application.h"
#include "ModuleResources.h"
#include "ResourceAudio.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceScene.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"



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
	ImportFilesToLibrary();
	return true;
}

bool ModuleResources::CleanUp()
{
	return true;
}

void ModuleResources::ImportFilesToLibrary()
{
	
	std::vector<std::string> files;
	std::vector<std::string> dirs;	
	
	App->fileSys->GetFilesFromDir(ASSETS_PATH, files, dirs, true);
	
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++ ) {
		ImportFile((*it).c_str());
	}
	
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

uuid ModuleResources::FindByName(const char * fileInAssets, Resource::ResType type) const
{
	std::string fileName = fileInAssets;

	for (std::map<uuid, Resource*>::const_iterator it = resources.begin(); it != resources.end(); it++) {
		if (it->second->GetNameStr() == fileName) {
			if (type == Resource::ResType::None || type == it->second->GetType())
			return it->first;
		}
	}
	return 0;
}

void  ModuleResources::ImportFile(const char * newFileInAssets)
{
	
	bool import_ok = false; 
	std::vector<std::string> exportedFiles;
	Resource::ResType type = GetResourceTypeFromExtension(newFileInAssets);
	switch (type) {
	case Resource::ResType::Texture: {import_ok = App->textures->ImportTexture(newFileInAssets, &exportedFiles); break; }
	case Resource::ResType::Scene: {import_ok = App->renderer3D->importer->ImportScene(newFileInAssets, &exportedFiles); break; }
	case Resource::ResType::Mesh: {import_ok = App->fileSys->CopyPEItoLib(newFileInAssets, &exportedFiles); break; }
	case Resource::ResType::Audio: {import_ok = App->fileSys->CopyAudioToLib(newFileInAssets, &exportedFiles); break; }
	}
	if (import_ok) {
		std::vector<uuid> uuids;
		for (int i = 0; i < exportedFiles.size(); i++) {
			Resource* res = CreateNewResource(type);
			std::string name;
			App->fileSys->GetNameFromPath(exportedFiles[i].c_str(), nullptr, &name, nullptr, nullptr);
			res->SetName(name.c_str());
			res->SetPath(newFileInAssets);
			res->SetExportedFile(exportedFiles[i].c_str());
			uuids.push_back(res->GetUUID());
		}		
			
		GenerateMetaFile(newFileInAssets, uuids);
		
	}
	
}

uuid ModuleResources::GenerateNewUUID()
{
	return 0;
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

Resource * ModuleResources::CreateNewResource(Resource::ResType type, uuid forceUUID)
{
	Resource* res = nullptr;
	uuid uuid;

	if (forceUUID != 0 && Get(forceUUID) == nullptr)
		uuid = forceUUID;
	else
		uuid = App->scene->GetRandomUUID();

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

void ModuleResources::GenerateMetaFile(const char* assetFile, std::vector<uuid> exportedUUIDs)
{
	Config meta;
	meta.AddUInt("UUIDs Num", exportedUUIDs.size());
	for (int i = 0; i < exportedUUIDs.size(); i++) {
		meta.AddUInt("UUID", exportedUUIDs[i]);
		Resource* res = Get(exportedUUIDs[i]);
		res->Save(meta);
	}
	meta.AddUInt("TimeStamp", 0);	

	char* buffer = nullptr;
	uint size = meta.Save(&buffer);
	std::string name;
	App->fileSys->GetNameFromPath(assetFile, nullptr, &name, nullptr, nullptr);	
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