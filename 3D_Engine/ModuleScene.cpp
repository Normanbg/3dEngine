#include "ModuleScene.h"
#include "GameObject.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled){

	name = "scene";
}

bool ModuleScene::Init(JSON_Object * obj)
{


	return true;
}

void ModuleScene::CreateGameObject(GameObject* gameObject){

	root->childrens;
	
}

update_status ModuleScene::Update(float dt)
{
	return UPDATE_CONTINUE;
}

