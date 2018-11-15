#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Quadtree.h"
#include "Resource.h"
#include "ModuleEditorCamera.h"
#include "Config.h"

#include "ModuleInput.h"////delete me

#include <vector>

ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled){

	name = "scene";
	

	pcg32_srandom_r(&rng, time(0), (intptr_t)&rng);
	
}

ModuleScene::~ModuleScene()
{

}
bool ModuleScene::Init(JSON_Object * obj)
{
	root = new GameObject("root");
	mainCamera = AddGameObject("Main Camera");
	mainCamera->AddComponent(CAMERA);
	mainCamera->GetComponentCamera()->SetFarPlaneDistance(85.f);
	mainCamera->GetComponentCamera()->LookAt({ 0.f, 0.f, 0.f });
	rootQuadTree = new Quadtree(AABB({ 0.f, 0.f, 0.f }, { 15.f, 15.f ,15.f }), 0);

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	bool ret = true;

	

	if(App->input->GetKey(SDL_SCANCODE_4)== KEY_DOWN) { ///// DEBUUG
		SaveScene();
	}
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) { ///// DEBUUG

		LoadScene();
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

/*
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
		
	mesh->num_vertex = cubeVertex.size();
	mesh->num_index = cubeIndices.size();
	mesh->GenerateBuffer();
	mesh->CreateBBox();
	ret->SetLocalAABB(mesh->bbox);
	return ret;
}
*/
GameObject * ModuleScene::GetGameObjectByUUID(uuid UUID) const
{
	GameObject* ret= nullptr;
	ret = GetGameObjectUUIDRecursive(UUID, root);
	return ret;
}

GameObject * ModuleScene::GetGameObjectUUIDRecursive(uuid UUID, GameObject * go) const
{
	GameObject* ret = go;
	if (ret->UUID == UUID) {
		return ret;
	}

	for (int i = 0; i < go->childrens.size(); i++) {
		ret = go->childrens[i];
		ret = GetGameObjectUUIDRecursive(UUID, ret);
		if (ret) {
			return ret;
		}
	}
	return nullptr;
}

uuid ModuleScene::GetRandomUUID()
{
	return pcg32_random_r(&rng);
}

void ModuleScene::ClearScene() const
{
	OWN_LOG("Clearing scene")
	for (int i = root->childrens.size() - 1; i > 0; i--) {
		if (root->childrens[i] == mainCamera)
			continue;
	
		root->RemoveChildren(root->childrens[i]);
	}
	for (int i = root->components.size() - 1; i > 0; i--) {
		root->RemoveComponent(root->components[i]);
	}
}

void ModuleScene::SaveScene(const char* file) 
{
	Config save;

	save.AddArray("GameObjects");
	root->Save(save);	

	char* buffer = nullptr;
	uint size = save.Save(&buffer);

	const char* fileName = nullptr;
	file == nullptr ? fileName = SCENE_FILE : fileName = file;
	App->fileSys->writeFile(fileName, buffer, size);

	fileName = nullptr;

	OWN_LOG("Saving scene.")
		RELEASE_ARRAY(buffer);
}

void ModuleScene::LoadScene(const char*file) 
{

	ClearScene();

	char* buffer = nullptr;
	const char* fileName = nullptr;
	file == nullptr ? fileName = SCENE_FILE : fileName = file;
	uint size = App->fileSys->readFile(fileName, &buffer);
	

	if (size < 0) {
		OWN_LOG("Error loading file %s. All data not loaded.", fileName)
		fileName = nullptr;
		return;
	}
	fileName = nullptr;
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
//
//void ModuleScene::AddGOtoQuadtree(GameObject * go)
//{
//}

void ModuleScene::SetQuadTree()
{
	rootQuadTree->Clear();
	GetAllStaticGOs(root);

	for (auto it : staticOBjs){
		SetQTSize(it);
	}
	for (auto j : staticOBjs) {
		AddGOtoQuadtree(j);
	}
}

void ModuleScene::GetAllStaticGOs(GameObject* go)
{
	if (go != root && go->staticGO)
		staticOBjs.push_back(go);
	for (auto it : go->childrens) {
		GetAllStaticGOs(it);
	}
}

void ModuleScene::AddGOtoQuadtree(GameObject * go)
{
	if (go == nullptr)
		return;

	if (go->GetComponentMesh()) {
		rootQuadTree->Insert(go);
	}
	for (auto it : go->childrens)
		AddGOtoQuadtree(it);
}

void ModuleScene::SetQTSize(GameObject* go) {
	if (go == nullptr)
		return;
	if (go->staticGO && go->GetComponentMesh())
	{
		rootQuadTree->quadTreeBox.Enclose(go->globalAABB);
		for (auto it : go->childrens)
		{
			SetQTSize(it);
		}
	}
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

	RELEASE(root);
	RELEASE(rootQuadTree);

	return true;
}


void ModuleScene::ShowGameObjectInspector(GameObject* newSelected)
{
	if (gObjSelected == newSelected)
		return;
	DeselectAll();
	gObjSelected = newSelected;

}

void ModuleScene::ShowTextureResourceInspector(uuid newResource)
{
	if (TextureResourceSelected == newResource)
		return;
	DeselectAll();
	TextureResourceSelected = newResource;

}

void ModuleScene::DeselectAll()
{	
	gObjSelected = nullptr;	
	TextureResourceSelected = 0;
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

void ModuleScene::Draw() {
	GameObject* iterator;

	std::vector<Component*> components;

	for (int i = 0; i < root->childrens.size(); i++) {
		iterator = root->childrens[i];
		iterator->GetComponents(MESH, components);
	}

	ComponentMesh* mesh;
	for (int i = 0; i < components.size(); i++) {
		mesh = (ComponentMesh *)components[i];

		if (App->camera->ContainsAaBox(mesh->myGO->globalAABB) == IS_IN || App->camera->ContainsAaBox(mesh->myGO->globalAABB) == INTERSECT) {//MISSING IF FRUSTUM CULLING ACTIVE!!!!!!---------------------------------------------------------
			mesh->Draw();
		}
	}
	if (!inGame && mainCamera != nullptr && rootQuadTree != nullptr) {
		mainCamera->GetComponentCamera()->DebugDraw();
		rootQuadTree->DebugDraw();
	}
	iterator = nullptr;
	mesh = nullptr;
}



GameObject * ModuleScene::AddGameObject()
{
	GameObject* ret = new GameObject();
	ret->parent = root;
	root->childrens.push_back(ret);
	return ret;
}


GameObject* ModuleScene::AddGameObject(const char* name) {
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



