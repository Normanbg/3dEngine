#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"

#include <vector>

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled){

	name = "scene";
	root = new GameObject("root");
}

ModuleScene::~ModuleScene()
{

}
bool ModuleScene::Init(JSON_Object * obj)
{
	
	//SceneImporter* sc = new SceneImporter;
	//sc->ImportFBXtoPEI("figure","figure");

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	bool ret = true;
	 
	if (root->childrens.empty() == false) {
		for (int i = 0; i < root->childrens.size(); i++) {
			ret &= root->childrens[i]->PreUpdate();
		}
	}

	update_status retUS;
	ret ? retUS = UPDATE_CONTINUE : retUS = UPDATE_ERROR;
	return retUS;
}

GameObject* ModuleScene::AddGameObject(const char* name){
	GameObject* ret = new GameObject(name);
	ret->parent = root;
	root->childrens.push_back(ret);
	
	return ret;
}

GameObject * ModuleScene::AddGameObject(const char * name, GameObject * parent)
{
	GameObject* ret = new GameObject(name);
	ret->parent = parent;
	parent->childrens.push_back(ret);

	return ret;
}

update_status ModuleScene::Update(float dt){

	bool ret = true;
	root->CalculateAllGlobalMatrix();


	if (root->childrens.empty() == false) {

		for (int i = 0; i < root->childrens.size(); i++) {
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
		for (int i = 0; i < root->childrens.size(); i++) {
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


void ModuleScene::AddGameObjectToSelectedList(GameObject* newSelected)
{
	if (!newSelected->GetSelected())
	{
		for (std::vector<GameObject*>::iterator it = gObjsSelected.begin(); it != gObjsSelected.end(); ++it)
		{
			if ((*it) == newSelected)
				return;
		}

		gObjsSelected.push_back(newSelected);
		newSelected->selected = true;
	}
}

void ModuleScene::DeselectAll()
{
		for (std::vector<GameObject*>::iterator it = gObjsSelected.begin(); it != gObjsSelected.end();)
		{
			(*it)->selected = false;
			it = gObjsSelected.erase(it);
		}
}

void ModuleScene::SetBoundingBox(bool active)
{
	std::vector<Component*> comp;
	root->GetComponents(MESH, comp);
	for (int i = 0; i < comp.size(); i++) {
		ComponentMesh* mesh = (ComponentMesh*) comp[i];
		mesh->ShowBBox = active;
	}
}

void ModuleScene::DrawMeshes() {
	GameObject* iterator;

	std::vector<Component*> components;

	for (int i = 0;i< root->childrens.size();i++){
		iterator = root->childrens[i];
		iterator->GetComponents(MESH,components);
		
	}
	ComponentMesh* mesh;
	for (int i = 0; i < components.size(); i++) {
		mesh = (ComponentMesh *) components[i];
		mesh->Draw();
	
	}

	iterator = nullptr;
	mesh = nullptr;
}