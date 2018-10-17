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
	return true;
}


ComponentMaterial* ModuleTextures::CreateComponentMaterial() {
	ComponentMaterial* ret;

	return ret;
}