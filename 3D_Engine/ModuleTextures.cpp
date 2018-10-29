#include "ModuleTextures.h"


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
		materials[i].CleanUp();
	}
	return true;
}

void ModuleTextures::AddMaterial(Material * tex)
{
	materials.push_back(*tex);
}

Material * ModuleTextures::GetMaterialsFromID(GLuint id)
{
	for (int i = 0; i < materials.size(); i++) {
		if (materials[i].textureID = id) {
			return &materials[i];
		}
	}
	OWN_LOG("Error getting texture from ID");
	return nullptr;
}

GLuint ModuleTextures::CheckIfImageAlreadyLoaded(const char * _path)
{
	for (int i = 0; i < materials.size(); i++) {
		if (strcmp(materials[i].path.c_str(), _path) == 0) {
			return materials[i].textureID;
		}
	}
	return -1;
}

void Material::CleanUp()
{
	if (textureID != -1)
		glDeleteBuffers(1, &textureID);
}
