#include "GameObject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

bool GameObject::PreUpdate(){
	bool ret = true;

	for (int i = 0; i < components.size() - 1; i++) {
		ret &= components[i]->PreUpdate();
	}
	   
	return ret;
}

bool GameObject::Update(){
	bool ret = true;

	for (int i = 0; i < components.size() - 1; i++) {
		ret &= components[i]->Update();
	}
	
	return ret;
}

bool GameObject::PostUpdate(){
	bool ret = true;

	for (int i = 0; i < components.size() - 1; i++) {
		ret &= components[i]->PostUpdate();
	}

	return ret;
}

void GameObject::CleanUp(){

	if (components.empty() == false) {
		for (int i = components.size() - 1; i >= 0; i--) {
			components[i]->CleanUp();
		}
	}
	components.clear();

	if (childrens.empty() == false) {
		for (int i = childrens.size() - 1; i >= 0; i--) {
			childrens[i]->CleanUp();
		}
	}
	childrens.clear();

	if (parent) {
		parent = nullptr;
	}
	
}

Component * GameObject::AddComponent(ComponentType type) {
	Component* ret;

	switch (type) {
	case ComponentType::MESH:
		ret = App->renderer3D->CreateComponentMesh();
		break;

	case ComponentType::MATERIAL:
		ret = App->textures->CreateComponentMaterial();
		break;

	case ComponentType::TRANSFORM:
		ret = CreateComponentTranformation();
		break;

	case ComponentType::NO_TYPE:
		return nullptr;
	}
	ret->myGO = this;
	components.push_back(ret);
	return ret;
}

ComponentTransformation * GameObject::CreateComponentTranformation()
{
	ComponentTransformation* ret;

	return ret;
}
