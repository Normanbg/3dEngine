#include "ModuleScene.h"
#include "GameObject.h"

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled){

	name = "scene";
	root = new GameObject();
}

ModuleScene::~ModuleScene()
{
	
}

bool ModuleScene::Init(JSON_Object * obj)
{


	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	bool ret = true;
	 
	if (root->childrens.empty() == false) {
		for (int i = 0; i < root->childrens.size() - 1; i++) {
			ret &= root->childrens[i]->PreUpdate();
		}
	}

	update_status retUS;
	ret ? retUS = UPDATE_CONTINUE : retUS = UPDATE_ERROR;
	return retUS;
}

GameObject* ModuleScene::AddGameObject(){
	GameObject* ret = new GameObject();
	ret->parent = root;
	root->childrens.push_back(ret);
	
	return ret;
}

update_status ModuleScene::Update(float dt){

	bool ret = true;
	root->CalculateAllGlobalMatrix();


	if (root->childrens.empty() == false) {
		for (int i = 0; i < root->childrens.size() - 1; i++) {
			ret &= root->childrens[i]->Update();
		}
	}
	update_status retUS;
	ret ? retUS = UPDATE_CONTINUE : retUS = UPDATE_ERROR;
	return retUS;
}

update_status ModuleScene::PostUpdate(float dt){

	bool ret = true;
	if (root->childrens.empty() == false) {
		for (int i = 0; i < root->childrens.size() - 1; i++) {
			ret &= root->childrens[i]->PostUpdate();
		}
	}
	update_status retUS;
	ret ? retUS = UPDATE_CONTINUE : retUS = UPDATE_ERROR;
	return retUS;
}

bool ModuleScene::CleanUp()
{	
	root->CleanUp();

	delete root;
	root = nullptr;

	return true;
}

