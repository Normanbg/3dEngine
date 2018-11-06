#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ModuleEditorCamera.h"
#include "ModuleTextures.h"
#include "Config.h"

#include "ModuleInput.h"////delte me

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
	if(App->input->GetKey(SDL_SCANCODE_4)== KEY_DOWN) { ///// DEBUUG

		SaveScene();
	}
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) { ///// DEBUUG

		LoadScene("Scenes/scene.json");
	}
	 
	if (root->childrens.empty() == false) {
		for (int i = 0; i < root->childrens.size(); i++) {
			ret &= root->childrens[i]->PreUpdate();
		}
	}


	update_status retUS;
	ret ? retUS = UPDATE_CONTINUE : retUS = UPDATE_ERROR;
	return retUS;
}
GameObject * ModuleScene::AddGameObject()
{
	GameObject* ret = new GameObject();
	ret->parent = root;
	root->childrens.push_back(ret);
	return ret;
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

GameObject * ModuleScene::GetGameObjectByUUID(uint uuid) const
{
	GameObject* ret= nullptr;
	ret = GetGameObjectUUIDRecursive(uuid, root);	
	return ret;
}

GameObject * ModuleScene::GetGameObjectUUIDRecursive(uint uuid, GameObject * go) const
{
	GameObject* ret = go;
	if (ret->uuid == uuid) {
		return ret;
	}

	for (int i = 0; i < go->childrens.size(); i++) {
		ret = go->childrens[i];
		ret = GetGameObjectUUIDRecursive(uuid, ret);
		if (ret) {
			return ret;
		}
	}
	return nullptr;
}

void ModuleScene::ClearScene() const
{
	OWN_LOG("Clearing scene")
		root->CleanUp();
}

bool ModuleScene::SaveScene() const
{
	bool ret = true;
	Config save;

	save.AddArray("GameObjects");
	root->Save(save);
	
	char* buffer = nullptr;
	uint size = save.Save(&buffer);
	App->fileSys->writeFile(SCENE_FILE, buffer, size);
	OWN_LOG("Saving scene.")
	RELEASE_ARRAY(buffer);
	
	return ret;
}

bool ModuleScene::LoadScene(const char* data)
{
	bool ret = true;

	if (data == nullptr) { return false; }

	ClearScene();

	char* buffer = nullptr;
	uint size = App->fileSys->readFile(data, &buffer);

	if (data !=nullptr ){
		if (size < 0) {
			OWN_LOG("Error loading file. All data not loaded.")
				return false;
		}
		Config conf(buffer);

		int num = conf.GetNumElemsArray("GameObjects");
		for (int i = 0; i < num; i++) {
			Config elem = conf.GetArray("GameObjects", i);
			GameObject* go = new GameObject();
			go->Load(&elem);
		}
		OWN_LOG("Loading new scene.")
		RELEASE_ARRAY(buffer);
	}
	else {
		OWN_LOG("Error loading file.")
			ret = false;
	}
	
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
	ClearScene();

	RELEASE(random);
	RELEASE(root);

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

	for (int i = 0; i < root->childrens.size(); i++) {
		iterator = root->childrens[i];
		iterator->GetComponents(MESH, components);
	}

	ComponentMesh* mesh;
	for (int i = 0; i < components.size(); i++) {
		mesh = (ComponentMesh *)components[i];
		if (App->camera->ContainsAaBox(mesh->boundingBox) == IS_IN || App->camera->ContainsAaBox(mesh->boundingBox) == INTERSECT) {//MISSING IF FRUSTUM CULLING ACTIVE!!!!!!---------------------------------------------------------
			mesh->Draw();
		}
	}
	iterator = nullptr;
	mesh = nullptr;
}

