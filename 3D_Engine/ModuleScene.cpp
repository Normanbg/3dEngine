#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ModuleEditorCamera.h"
#include "ModuleTextures.h"

#include <vector>

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled){

	name = "scene";
	

	random = new math::LCG();
	
}

ModuleScene::~ModuleScene()
{

}
bool ModuleScene::Init(JSON_Object * obj)
{
	root = new GameObject("root");

	

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

GameObject * ModuleScene::CreateCube()
{
	GameObject* ret = nullptr;
	if (numCubes == 0)
	{
		ret = App->scene->AddGameObject("Cube");
	}
	else
	{
		char cubeNumb[30];
		sprintf_s(cubeNumb, 30, "Cube_%d", App->scene->numCubes);
		ret = App->scene->AddGameObject(cubeNumb);
	}
	App->scene->numCubes++;
	ret->AddComponent(MESH);

	std::vector<float3> cubeVertex = { float3{.0f,.0f,.0f},  {1.0f,.0f,.0f} ,{.0f,1.0f,.0f} , {1.0f,1.0f,.0f} , {.0f,.0f,1.0f} , {1.0f,.0f,1.0f} , {.0f,1.0f,1.0f}  ,  {1.0f,1.0f,1.0f} };
	std::vector<uint> cubeIndices = { 0,1,2 , 1,3,2 , 3,1,5 , 5,7,3 , 7,5,4 , 6,7,4 , 6,4,0, 0,2,6  , 6,2,3 , 6,3,7 , 0,4,5 , 0,5,1 };
	
	ComponentMesh* mesh = ret->GetComponentMesh();
	
	mesh->vertex = new float3[8];
	mesh->index = new uint[36];

	memcpy(mesh->vertex, &cubeVertex[0], sizeof(float3) * 8);
	memcpy(mesh->index, &cubeIndices[0], sizeof(uint) * 36);
		
	ret->GetComponentMesh()->num_vertex = cubeVertex.size();
	ret->GetComponentMesh()->num_index = cubeIndices.size();
	ret->GetComponentMesh()->GenerateBuffer();
	return ret;
}

void ModuleScene::SaveScene(JSON_Object * data) const
{
	/*
	JSON_Value* value = json_value_init_object();
	JSON_Object* obj = json_value_get_object(value);

	json_object_dotset_string(obj, "App.Name", window->GetWindowTitle().c_str());
	json_object_dotset_string(obj, "App.Organization", GetOrganization().c_str());


	std::list<Module*>::const_iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		ret &= (*item)->Save(obj);
		item++;
	}

	json_serialize_to_file(value, CONFIG_FILE);
	json_object_clear(obj);
	json_value_free(value);

	*/
}

void ModuleScene::LoadScene(JSON_Object * data)
{
	/*
	JSON_Value* config;

	if (config = json_parse_file(CONFIG_FILE)) {
		OWN_LOG("Config.JSON File detected");
		JSON_Object* obj;
		JSON_Object* appObj;


		obj = json_value_get_object(config);
		appObj = json_object_get_object(obj, "App");

		SetDataFromJson(appObj);


		json_object_clear(appObj);

		std::list<Module*>::iterator item = list_modules.begin();

		while (item != list_modules.end())
		{
			ret &= (*item)->Load(json_object_get_object(obj, (*item)->name.c_str()));
			item++;
		}

		json_object_clear(obj);
	}

	json_value_free(config);
*/

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

	delete random;
	random = nullptr;

	delete root;
	root = nullptr;

	return true;
}


void ModuleScene::ShowGameObjectInspector(GameObject* newSelected)
{
	if (gObjSelected == newSelected)
		return;
	DeselectAll();
	gObjSelected = newSelected;

}

void ModuleScene::ShowMaterialInspector(Material * newSelected)
{
	if (materialSelected == newSelected)
		return;
	DeselectAll();
	materialSelected = newSelected;

}

void ModuleScene::DeselectAll()
{	
	gObjSelected = nullptr;	
	materialSelected = nullptr;
}

void ModuleScene::SetBoundingBox(bool active)
{
	GameObject* iterator;
	std::vector<Component*> comp;
	
	for (int i = 0; i < root->childrens.size(); i++) {
		iterator = root->childrens[i];
		iterator->GetComponents(MESH, comp);
	}
	ComponentMesh* mesh;
	for (int i = 0; i < comp.size(); i++) {
		mesh = (ComponentMesh *)comp[i];
		mesh->showBBox = active;

	}
}

void ModuleScene::SetWireframe(bool active)
{
	GameObject* iterator;
	std::vector<Component*> comp;

	for (int i = 0; i < root->childrens.size(); i++) {
		iterator = root->childrens[i];
		iterator->GetComponents(MESH, comp);
	}
	ComponentMesh* mesh;
	for (int i = 0; i < comp.size(); i++) {
		mesh = (ComponentMesh *)comp[i];
		mesh->showWireframe = active;

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
		mesh = (ComponentMesh *)components[i];
		if (App->camera->ContainsAaBox(mesh->boundingBox) == IS_IN || App->camera->ContainsAaBox(mesh->boundingBox) == INTERSECT) {//MISSING IF FRUSTUM CULLING ACTIVE!!!!!!---------------------------------------------------------
			mesh->Draw();
		}
		else if (App->camera->ContainsAaBox(mesh->boundingBox) == IS_OUT) {
		//OWN_LOG("out motherfucker");
		}
	}
	iterator = nullptr;
	mesh = nullptr;
}