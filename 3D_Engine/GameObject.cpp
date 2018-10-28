#include "GameObject.h"
#include <vector>
#include <string>


GameObject::GameObject(const char * Name)
{
	name = Name;
	transformComp = new ComponentTransformation();
	transformComp->type = TRANSFORM;
	components.push_back(transformComp);
}

GameObject::~GameObject()
{
}

bool GameObject::PreUpdate(){
	bool ret = true;

	for (int i = 0; i < components.size(); i++) {

		ret &= components[i]->PreUpdate();
	}
	for (int i = 0; i < childrens.size() ; i++) {

		ret &= components[i]->PreUpdate();
	}
	   
	return ret;
}

bool GameObject::Update(){
	bool ret = true;


	for (int i = 0; i < components.size() ; i++) {
		ret &= components[i]->Update();
	}
	for (int i = 0; i < childrens.size() ; i++) {
		ret &= childrens[i]->Update();
	}
	return ret;
}

bool GameObject::PostUpdate(){
	bool ret = true;

	for (int i = 0; i < components.size(); i++) {

		ret &= components[i]->PostUpdate();
	}
	for (int i = 0; i < childrens.size(); i++) {

		ret &= childrens[i]->PostUpdate();
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

void GameObject::CalculateAllGlobalMatrix(){
	if (parent == nullptr)
	{
		transformComp->globalMatrix = transformComp->localMatrix;
	}
	else
		transformComp->globalMatrix = parent->transformComp->globalMatrix * transformComp->localMatrix;

	if (!childrens.empty())
	{
		for (std::vector<GameObject*>::iterator it = childrens.begin(); it != childrens.end(); it++)
		{
			(*it)->CalculateAllGlobalMatrix();
		}
	}
}

Component * GameObject::AddComponent(ComponentType type) {
	Component* ret;

	switch (type) {
	case ComponentType::MESH:
		ret = new ComponentMesh();

		ret->type = MESH;

		break;

	case ComponentType::MATERIAL:
		ret = new ComponentMaterial();
		ret->type = MATERIAL;

		break;

	case ComponentType::TRANSFORM:
		ret = new ComponentTransformation();

		ret->type = TRANSFORM;
		break;
	
	case ComponentType::CAMERA:
		ret = new ComponentCamera();
		ret->type = CAMERA;
		break;

	case ComponentType::NO_TYPE:
		return nullptr;
	}
	ret->myGO = this;
	components.push_back(ret);
	return ret;
}


void GameObject::GetComponents(ComponentType type, std::vector<Component*>& comp) {
	Component* iterator;
	for (int i = 0; i < components.size(); i++) {
		iterator = components[i];
		if (iterator->type == type) {
			comp.push_back(iterator);
		}
	}
	GameObject* gameObject;
	for(int i = 0; i < childrens.size(); i++) {
		gameObject = childrens[i];
		gameObject->GetComponents(type, components);
	}
	iterator = nullptr;
}

bool GameObject::GetSelected()
{
	return selected;
}

void GameObject::SetParent(GameObject * parent)
{
	if (this->parent != parent) {
		this->parent = parent;
		parent->AddChildren(this);
	}
}

void GameObject::AddChildren(GameObject * child)
{
	this->childrens.push_back(child);
	child->SetParent(this);
}

void GameObject::DrawMeshes()
{
	Component* iterator;
	for (int i = 0; i < components.size(); i++) {
		iterator = components[i];
		
	}
	iterator = nullptr;
}



ComponentTransformation * GameObject::GetTransformComponent()
{
	ComponentTransformation* ret = nullptr;
	//WILL ONLY FIND THE FIRST COMPONENT EQUAL TO TYPE OF EACH G0
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == TRANSFORM)
			return (ComponentTransformation*)(*it);
	}
	return ret;
}

ComponentCamera * GameObject::GetComponentCamera()
{
	ComponentCamera* ret = nullptr;
	//WILL ONLY FIND THE FIRST COMPONENT EQUAL TO TYPE OF EACH G0
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == CAMERA)
			return (ComponentCamera*)(*it);
	}
	return ret;
}

bool GameObject::isSelected()
{
	return selected;
}

void GameObject::setName(char * _name)
{
	name = _name;
}

void GameObject::ToggleSelected(){
	selected = !selected;
}

