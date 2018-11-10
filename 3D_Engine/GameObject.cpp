#include "GameObject.h"
#include "Config.h"

#include <array>
#include <vector>
#include <string>


GameObject::GameObject()
{
	transformComp = new ComponentTransformation();
	transformComp->type = TRANSFORM;
	transformComp->myGO = this;
	components.push_back(transformComp);

	localAABB.SetNegativeInfinity();
	globalAABB.SetNegativeInfinity();

	uuid = App->scene->GetRandomUUID();
}

GameObject::GameObject(const char * Name)
{
	name = Name;
	transformComp = new ComponentTransformation();
	transformComp->type = TRANSFORM;
	transformComp->myGO = this;
	components.push_back(transformComp);

	localAABB.SetNegativeInfinity();
	globalAABB.SetNegativeInfinity();

	uuid = App->scene->GetRandomUUID();	
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

	OBB newobb = localAABB;
	newobb.Transform(transformComp->globalMatrix);

	obb = newobb;
	globalAABB.SetNegativeInfinity();
	globalAABB.Enclose(obb);

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

void GameObject::AddComponent(Component * component, ComponentType type)
{	
	component->type = type;
	component->myGO = this;
	components.push_back(component);
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




void GameObject::SetParent(GameObject * _parent)//TO CHECK!!!----------------------------------------------------------
{
		
	if (parent != nullptr ) {
		if (parent == _parent) {
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

void GameObject::RemoveChildren(GameObject * toRemove)
{
	for (int i = 0; i < childrens.size(); i++) {
		if (childrens[i] == toRemove) {
			childrens[i]->CleanUp();
			childrens.erase(childrens.begin() + i);
			return;
		}
	}
}

void GameObject::RemoveComponent(Component * comp)
{
	for (int i = 0; i < components.size(); i++) {
		if (components[i] == comp ) {
			components[i]->CleanUp();
			components.erase(components.begin()+i);
			return;
		}
	}
}


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

ComponentMaterial * GameObject::GetComponentMaterial(const uint uuid)
{
	std::vector<Component*> materials;

	GetComponents(MATERIAL, materials);
	for (int i = 0; i < materials.size(); i++){
		ComponentMaterial* it = (ComponentMaterial*)materials[i];
		const uint auxuuid = it->GetUUID();
		if (auxuuid == uuid) {
			return it;
		}
		it = nullptr;
	}
	return nullptr;
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

void GameObject::setChildsStatic()
{
	for (auto it : childrens)
	{
		it->staticGO = true;
		it->setChildsStatic();
	}
}

void GameObject::SetLocalAABB(AABB aabb)
{
	AABB compare;
	compare.SetNegativeInfinity();
	if (localAABB.minPoint.Equals(compare.minPoint) && localAABB.minPoint.Equals(compare.minPoint))
	{
		localAABB = aabb;
		obb = localAABB;
	}
	else
		localAABB.Enclose(aabb);
}

void GameObject::ToggleSelected(){
	inspectorSelected = !inspectorSelected;
}


void GameObject::Save(Config& data) const
{
	Config conf;
		

	conf.AddUInt("UUID", uuid);
	if (parent != nullptr) { // in case of root
		conf.AddUInt("Parent_UUID", parent->uuid);
	}
	conf.AddString("Name", name.c_str());
	conf.AddFloat3("Translation", transformComp->getPos());
	conf.AddFloat3("Scale", transformComp->getScale());
	conf.AddFloat3("Rotation", transformComp->getEulerRot()); //save rotation as eulerangle(float3) to save memory.

	conf.AddArray("Components");	

	for (int i = 0; i < components.size(); i++) { //iterate all over the components to save
		Config comp;
		comp.AddInt("Type", components[i]->type);
		components[i]->Save(comp);
		conf.AddArrayChild(comp);
	}	

	data.AddArrayChild(conf);

	for (int i = 0; i < childrens.size(); i++) {//iterate all over the childs to save
		childrens[i]->Save(data);
	}

}

void GameObject::Load(Config* data)
{
	uuid = data->GetUInt("UUID", 0);
	uint parentUUID = data->GetUInt("Parent_UUID",0);

	GameObject* par = App->scene->GetGameObjectByUUID(parentUUID);
	if (par != nullptr) {
		SetParent(par);
	}
	par = nullptr;
	name = data->GetString("Name", "NoName");
	if (strcmp(name.c_str(), "root")==0) {
		App->scene->ChangeRootGO( this);
	}
	transformComp->setPos(data->GetFloat3("Translation", {0,0,0}));
	transformComp->setRotEuler(data->GetFloat3("Rotation", { 0,0,0 }));
	transformComp->setScale(data->GetFloat3("Scale", { 0,0,0 }));
	//Need to apply the transformation?

	int num = data->GetNumElemsArray("Components");
	for (int i = 0; i < num; i++) {//iterate all over the childs to save (ecept transform comp)
		Config elem = data->GetArray("Components", i);
		ComponentType type = (ComponentType) elem.GetInt("Type", ComponentType::NO_TYPE);
		if (type != ComponentType::NO_TYPE ) {
			if (type != ComponentType::TRANSFORM) {
				Component* comp = AddComponent(type);
				comp->Load(&elem);
			}
		}
		else {
			OWN_LOG("Cannot load components correctly. Component type: NOTYPE ")
		}
	}

	
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
