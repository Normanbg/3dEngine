#include "Application.h"
#include "ModuleResources.h"
#include "ResourceAudio.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceScene.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"

#include <filesystem>
#include <iostream>


ModuleResources::ModuleResources() 
{
}


ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init(JSON_Object * obj)
{
	ImportFilesToLibrary();

	return true;
}

bool ModuleResources::Start()
{
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
	
	
	App->fileSys->GetFilesFromDir(ASSETS_PATH, files, dirs);
	while (dirs.size() >0) {
		uint lastDirs = dirs.size();
		std::string path = ASSETS_PATH;
		path += dirs[dirs.size() - 1].c_str();
		path += "/";
		App->fileSys->GetFilesFromDir(path.c_str(), files, dirs);
		uint diff = dirs.size() != lastDirs;
		if (diff>0) {
			for (int i = 0; i < diff; i--) {
				std::string name = dirs[dirs.size() - 1 - diff];
				//working on it
				dirs.pop_back();
			}
		}
		dirs.pop_back();

	}
	
	
}

uuid ModuleResources::Find(const char * fileInAssets) const
{
	std::string fileName = fileInAssets;
	
	for (std::map<uuid, Resource*>::const_iterator it = resources.begin(); it != resources.end(); it++) {
		if (it->second->GetFileStr() == fileName) {
			return it->first;
		}	
	}
	return 0;
}

uuid ModuleResources::ImportFile(const char * newFileInAssets)
{
	uuid ret = 0;
	bool import_ok = false; 
	std::string writtenFile;
	Resource::ResType type = GetResourceTypeFromExtension(newFileInAssets);
	switch (type) {
	case Resource::ResType::Texture: {import_ok = App->textures->ImportTexture(newFileInAssets); break; }
	case Resource::ResType::Scene: {import_ok = App->renderer3D->importer->ImportScene(newFileInAssets); break; }
	case Resource::ResType::Mesh: {import_ok = App->fileSys->CopyPEItoLib(newFileInAssets); break; }
	case Resource::ResType::Audio: {import_ok = App->fileSys->CopyAudioToLib(newFileInAssets); break; }
	}
	if (import_ok) {
		Resource* res = CreateNewResource(type);
		res->SetFile(newFileInAssets);
		res->SetExportedFile(writtenFile.c_str());
		ret = res->GetUUID();
	}
	return ret;
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
	case Resource::ResType::Scene: {res = (Resource*) new ResourceScene(uuid); break;	}
	}
	if (res != nullptr) {
		resources[uuid] = res;
	}
	return res;
}

const Resource::ResType ModuleResources::GetResourceTypeFromExtension(const char * path) const
{	
	std::string extension;
	App->fileSys->GetNameFromPath(path, nullptr, nullptr, nullptr, &extension);
	if (extension == OWN_FILE_FORMAT)  return Resource::ResType::Mesh;
	if (extension == FBX_FORMAT)  return Resource::ResType::Scene;
	if (extension == DDS_FORMAT)  return Resource::ResType::Texture;
	if (extension == PNG_FORMAT)  return Resource::ResType::Texture;
	if (extension == JPG_FORMAT)  return Resource::ResType::Texture;
	if (extension == JPEG_FORMAT)  return Resource::ResType::Texture;
	if (extension == TGA_FORMAT)  return Resource::ResType::Texture;
	if (extension == WAV_FORMAT)  return Resource::ResType::Audio;
	if (extension == OGG_FORMAT)  return Resource::ResType::Audio;
	
	return Resource::ResType::None;
}