#include "GameObject.h"

#include <array>
#include <vector>
#include <string>


GameObject::GameObject(const char * Name)
{
	name = Name;

	//For the transform comp the Addcomp is useless
	transformComp = new ComponentTransformation();
	transformComp->type = TRANSFORM;
	transformComp->myGO = this;
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

		ret &= childrens[i]->PreUpdate();
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
		gameObject->GetComponents(type, comp);
	}
	iterator = nullptr;
}

bool GameObject::GetSelected()
{
	return selected;
}

void GameObject::SetParent(GameObject * _parent)//TO CHECK!!!----------------------------------------------------------
{
		
	if (parent != nullptr ) {
		if (parent = _parent) {
			return;
		}		
		for (std::vector<GameObject*>::iterator iterator = parent->childrens.begin(); iterator != parent->childrens.end(); iterator++) {
			if (this == (*iterator)) {
				parent->childrens.erase(iterator);
				break;
			}
		}		
	}
	parent = _parent;
	parent->AddChildren(this);
}

void GameObject::AddChildren(GameObject * child)
{
	for (int i = 0; i < childrens.size(); i++) {
		if (childrens[i] == child) {
			return;
		}
	}
	this->childrens.push_back(child);
	child->SetParent(this);
}
//
//void GameObject::DrawMeshes()
//{
//	Component* iterator;
//	for (int i = 0; i < components.size(); i++) {
//		iterator = components[i];
//		
//	}
//	iterator = nullptr;
//}



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

ComponentMesh * GameObject::GetComponentMesh()
{
	ComponentMesh* ret = nullptr;
	//WILL ONLY FIND THE FIRST COMPONENT EQUAL TO TYPE OF EACH G0
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == MESH)
			return (ComponentMesh*)(*it);
	}
	return ret;
}

ComponentMaterial * GameObject::GetComponentMaterial()
{
	ComponentMaterial* ret = nullptr;
	//WILL ONLY FIND THE FIRST COMPONENT EQUAL TO TYPE OF EACH G0
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == MATERIAL)
			return (ComponentMaterial*)(*it);
	}
	return ret;
}
bool GameObject::isSelected()
{
	return selected;
}
//
//bool GameObject::isActive()
//{
//	return active;
//}
//
//bool GameObject::isStatic()
//{
//	return staticGO;
//}

void GameObject::setName(char * _name)
{
	name = _name;
}

void GameObject::ToggleSelected(){
	selected = !selected;
}
//
//void GameObject::ToggleActive()
//{
//	active = !active;
//}
//
//void GameObject::ToggleStatic()
//{
//	staticGO = !staticGO;
//}
//
//void GameObject::CreateCube(GameObject* &toConvert)
//{
//	if (App->scene->numCubes == 0)
//	{
//		toConvert = App->scene->AddGameObject("Cube");
//	}
//	else
//	{
//		App->scene->numCubes++;
//		std::string strCube = "Cube" + App->scene->numCubes;
//		toConvert = App->scene->AddGameObject(strCube.c_str());
//	}
//	toConvert->AddComponent(MESH);
//
//	std::vector<float3> cubeVertex = { float3{.0f,.0f,.0f},  {1.0f,.0f,.0f} ,{.0f,1.0f,.0f} , {1.0f,1.0f,.0f} , {.0f,.0f,1.0f} , {1.0f,.0f,1.0f} , {.0f,1.0f,1.0f}  ,  {1.0f,1.0f,1.0f} };
//	std::vector<uint> cubeIndices = { 0,1,2 , 1,3,2 , 3,1,5 , 5,7,3 , 7,5,4 , 6,7,4 , 6,4,0, 0,2,6  , 6,2,3 , 6,3,7 , 0,4,5 , 0,5,1 };
//	/*memcpy(&ret->GetComponentMesh()->vertex[0], &cubeVertex[0], sizeof(float3) * 8);
//	memcpy(&ret->GetComponentMesh()->index[0], &cubei[0], sizeof(uint) * 36);*/
//	for (int i = 0; i < cubeVertex.size(); i++)
//	{
//		toConvert->GetComponentMesh()->vertex[i] = cubeVertex[i];
//	}
//	for (int j = 0; j < cubeIndices.size(); j++)
//	{
//		toConvert->GetComponentMesh()->index[j] = cubeIndices[j];
//	}
//	toConvert->GetComponentMesh()->num_vertex = cubeVertex.size();
//	toConvert->GetComponentMesh()->num_index = cubeIndices.size();
//	toConvert->GetComponentMesh()->GenerateBuffer();
//}
