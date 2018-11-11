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
	return true;
}

bool ModuleResources::CleanUp()
{
	return true;
}

UUID ModuleResources::Find(const char * fileInAssets) const
{
	std::string fileName = fileInAssets;
	
	for (std::map<UUID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); i++) {
		if (it->second->GetFileStr() == fileName) {
			return it->first;
		}	
	}
	return 0;
}

UUID ModuleResources::ImportFile(const char * newFileInAssets)
{
	UUID ret = 0;
	bool import_ok = false; 
	std::string writtenFile;
	Resource::ResType type = GetResourceTypeFromExtension(newFileInAssets);
	switch (type) {
	case Resource::ResType::Texture: {import_ok = App->renderer3D->texImporter->ImportToDDS(newFileInAssets); break; }
	case Resource::ResType::Scene: {break; }
	case Resource::ResType::Mesh: {break; }
	case Resource::ResType::Audio: {break; }
	}
	if (import_ok) {
		Resource* res = CreateNewResource(type);
		res->SetFile(newFileInAssets);
		res->SetExportedFile(writtenFile.c_str());
		ret = res->GetUUID();
	}
	return ret;
}

UUID ModuleResources::GenerateNewUUID()
{
	return 0;
}

const Resource * ModuleResources::Get(UUID uuid) const
{
	for (std::map<UUID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); i++) {
		if (it->first == uuid) {
			return it->second;
		}
	}
	return nullptr;
}

Resource * ModuleResources::Get(UUID uuid)
{
	for (std::map<UUID, Resource*>::iterator it = resources.begin(); it != resources.end(); i++) {
		if (it->first == uuid) {
			return it->second;
		}
	}
	return nullptr;
}

Resource * ModuleResources::CreateNewResource(Resource::ResType type, UUID forceUUID)
{
	Resource* res = nullptr;
	UUID uuid;

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