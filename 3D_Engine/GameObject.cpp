#include "GameObject.h"
#include "Config.h"
#include "ResourceMesh.h"

#include <array>
#include <vector>
#include <string>

#include "mmgr/mmgr.h"

GameObject::GameObject()
{
	//transformComp = new ComponentTransformation();
	//transformComp->myGO = this;
	//components.push_back(transformComp);

	localAABB.SetNegativeInfinity();
	globalAABB.SetNegativeInfinity();

	UUID = App->scene->GetRandomUUID();
}

GameObject::GameObject(const char * Name)
{
	name = Name;

	//transformComp = new ComponentTransformation();
	//transformComp->myGO = this;
	//components.push_back(transformComp);


	localAABB.SetNegativeInfinity();
	globalAABB.SetNegativeInfinity();

	UUID = App->scene->GetRandomUUID();	
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
			RELEASE(components[i]);
		}
	}
	components.clear();

	if (childrens.empty() == false) {
		for (int i = childrens.size() - 1; i >= 0; i--) {
			childrens[i]->CleanUp();
			RELEASE(childrens[i]);
		}
	}
	childrens.clear();

	if (parent) {
		parent = nullptr;
	}
	//transformComp = nullptr;
}

void GameObject::CalculateAllGlobalMatrix(){
	if (parent == nullptr)
	{
		GetComponentTransform()->globalMatrix = GetComponentTransform()->localMatrix;
	}
	else
		GetComponentTransform()->globalMatrix = parent->GetComponentTransform()->globalMatrix * GetComponentTransform()->localMatrix;

	OBB newobb = localAABB;
	newobb.Transform(GetComponentTransform()->globalMatrix);

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
		break;

	case ComponentType::MATERIAL:
		ret = new ComponentMaterial();
		break;

	case ComponentType::TRANSFORM:
		ret = new ComponentTransformation();
		break;

	case ComponentType::TRANSFORMRECT:
		ret = new ComponentRectTransform();
		break;

	case ComponentType::CAMERA:
		ret = new ComponentCamera();
		break;

	case ComponentType::CANVAS:
		ret = new ComponentCanvas();
		ret->type = CANVAS;
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


ComponentTransformation * GameObject::GetComponentTransform() const
{
	ComponentTransformation* ret = nullptr;
	//WILL ONLY FIND THE FIRST COMPONENT EQUAL TO TYPE OF EACH G0
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == TRANSFORM)
			return (ComponentTransformation*)(*it);
	}
	return ret;
}

ComponentCamera * GameObject::GetComponentCamera() const
{
	ComponentCamera* ret = nullptr;
	//WILL ONLY FIND THE FIRST COMPONENT EQUAL TO TYPE OF EACH G0
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == CAMERA)
			return (ComponentCamera*)(*it);
	}
	return ret;
}

ComponentMesh * GameObject::GetComponentMesh() const 
{
	ComponentMesh* ret = nullptr;
	//WILL ONLY FIND THE FIRST COMPONENT EQUAL TO TYPE OF EACH G0
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == MESH)
			return (ComponentMesh*)(*it);
	}
	return ret;
}

ComponentCanvas * GameObject::GetComponentCanvas() const
{
	ComponentCanvas* ret = nullptr;
	//WILL ONLY FIND THE FIRST COMPONENT EQUAL TO TYPE OF EACH G0
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->type == CANVAS)
			return (ComponentCanvas*)(*it);
	}
	return ret;
}

ComponentMaterial * GameObject::GetComponentMaterial(const uuid UUID)
{
	std::vector<Component*> materials;

	GetComponents(MATERIAL, materials);
	for (int i = 0; i < materials.size(); i++){
		ComponentMaterial* it = (ComponentMaterial*)materials[i];
		const uint auxuuid = it->GetUUID();
		if (auxuuid == UUID) {
			return it;
		}
		it = nullptr;
	}
	return nullptr;
}

const bool GameObject::GetSelected() const
{
	return inspectorSelected;
}

ComponentMaterial * GameObject::GetComponentMaterial() const 
{
	ComponentMaterial* ret = nullptr;
	//WILL ONLY FIND THE FIRST COMPONENT EQUAL TO TYPE OF EACH G0
	for (std::vector<Component*>::const_iterator it = components.begin(); it != components.end(); it++)
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

void GameObject::SetName(char * _name)
{
	name = _name;
}

void GameObject::SetChildsStatic(bool active)
{
	if (this->staticGO != active && active == true)
		App->scene->staticsGObjs++;
	else if (this->staticGO != active && active == false)
		App->scene->staticsGObjs--;
	for (auto it : childrens)
	{
		it->SetChildsStatic(active);
		it->staticGO = active;
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

void GameObject::RayHits(const LineSegment & segment, bool & hit, float & dist){
	hit = false;
	dist = 99999999999.f;

	if (globalAABB.IsFinite()) {
		if (segment.Intersects(globalAABB)) {
			ComponentMesh* mesh;
			mesh = GetComponentMesh();
			if (mesh && mesh->HasMesh()) {
				if (mesh->GetResourceMesh()->vertex == nullptr)
					return;
				//Segment for the mesh
				LineSegment localRay(segment);
				localRay.Transform(GetComponentTransform()->getGlobalMatrix().Inverted());

				uint* indices = mesh->GetResourceMesh()->index;
				float3* vertices = mesh->GetResourceMesh()->vertex;
				Triangle triangle;

				for (int i = 0; i < mesh->GetResourceMesh()->num_index;) {
					//TO CHEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEECK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					triangle.a = vertices[indices[i]]; ++i;
					triangle.b = vertices[indices[i]]; ++i;
					triangle.c = vertices[indices[i]]; ++i;

					float distance;
					float3 hitPoint;

					if (localRay.Intersects(triangle, &distance, &hitPoint))
					{
						if (distance < dist)
						{
							dist = distance;
							hit = true;
						}
					}
				}
			}
		}
	}
}

void GameObject::ToggleSelected(){
	inspectorSelected = !inspectorSelected;
}


void GameObject::Save(Config& data) const
{
	Config conf;
		

	conf.AddUInt("UUID", UUID);
	if (parent != nullptr) { // in case of root
		conf.AddUInt("Parent_UUID", parent->UUID);
	}
	conf.AddString("Name", name.c_str());	
	conf.AddBool("Static", staticGO);
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
	UUID = data->GetUInt("UUID", 0);
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
	if (strcmp(name.c_str(), "Main Camera") == 0) {
		App->scene->SetMainCamera(this);
	}
	
	staticGO = data->GetBool("Static", false);

	int num = data->GetNumElemsArray("Components");
	for (int i = 0; i < num; i++) {//iterate all over the childs to save (ecept transform comp)
		Config elem = data->GetArray("Components", i);
		ComponentType type = (ComponentType) elem.GetInt("Type", ComponentType::NO_TYPE);
		if (type != ComponentType::NO_TYPE) {

			Component* comp = AddComponent(type);
			comp->Load(&elem);
		}
		else {
			OWN_LOG("Cannot load components correctly. Component type: NOTYPE ")
		}
	}

	
}


