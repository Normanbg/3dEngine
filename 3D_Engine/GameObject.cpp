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

Component * GameObject::AddComponent(ComponentType type){
	Component* ret;

	switch (type) {
	case ComponentType::MESH:
		ret = App->renderer3D->CreateComponentMesh();
	}

	ret->myGO = this;
	components.push_back(ret);
	return ret;
}
