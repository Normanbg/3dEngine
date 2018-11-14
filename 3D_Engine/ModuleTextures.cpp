#include "ModuleTextures.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer3D.h"

ModuleTextures::ModuleTextures(bool start_enabled) : Module(start_enabled)
{
	name = "textures";
}

ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Init(JSON_Object * obj)
{
	return true;
}

update_status ModuleTextures::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTextures::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTextures::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp()
{
	for (int i = materials.size() - 1; i >= 0; i--) {
		materials[i]->CleanUp();
	}
	return true;
}

void ModuleTextures::AddMaterial(Material * tex)
{
	materials.push_back(tex);
}

Material * ModuleTextures::GetMaterialsFromID(GLuint id)
{
	for (int i = 0; i < materials.size(); i++) {
		if (materials[i]->textureID == id) {
			return materials[i];
		}
	}
	OWN_LOG("Error getting texture from ID");
	return nullptr;
}

Material * ModuleTextures::GetMaterialsFromName(const char * name)
{
	for (int i = 0; i < materials.size(); i++) {
		if (strcmp(materials[i]->name.c_str(), name) == 0) {
			return materials[i];
		}
	}
	OWN_LOG("Error getting texture from ID");
	return nullptr;
}

GLuint ModuleTextures::CheckIfImageAlreadyLoaded(const char * name)
{
	for (int i = 0; i < materials.size(); i++) {
		if (strcmp(materials[i]->name.c_str(), name) == 0) {
			return materials[i]->textureID;
		}
	}
	return -1;
}

bool ModuleTextures::ImportTexture(const char* tex, std::string* written)
{
	bool ret = false;
	
	std::string extension;
	App->fileSys->GetNameFromPath(tex, nullptr, nullptr, nullptr, &extension);
	if (extension != DDS_FORMAT) 
		ret = App->renderer3D->texImporter->ImportToDDS(tex, nullptr, written);	
	else 
		ret = App->fileSys->CopyDDStoLib(tex, written);	

	return ret;
}

void ModuleTextures::LoadDroppedTexture(char * droppedFileDire)
{
	
	
	std::string texName;
	std::string extension;
	
	App->fileSys->GetNameFromPath(droppedFileDire, nullptr, &texName, nullptr, nullptr);
	App->fileSys->GetNameFromPath(droppedFileDire, nullptr, nullptr, nullptr, &extension);
	if (extension != DDS_FORMAT) {
		App->renderer3D->texImporter->ImportToDDS(droppedFileDire, texName.c_str());
	}
	
	if (App->textures->CheckIfImageAlreadyLoaded(texName.c_str()) == -1) {
		Material* mat = new Material;
		if (extension == DDS_FORMAT) {
			mat->textureID = App->renderer3D->texImporter->LoadTexture(droppedFileDire, mat);
		}
		else {
			std::string texPath;

			texPath = LIB_TEXTURES_PATH + texName + DDS_FORMAT;
			mat->textureID = App->renderer3D->texImporter->LoadTexture(texPath.c_str(), mat);
		}
		mat->name = texName;
		App->textures->AddMaterial(mat);
	}
	else {
		OWN_LOG("Material already loeaded");
	}
}





void Material::CleanUp()
{
	if (textureID != -1)
		glDeleteBuffers(1, &textureID);
}
