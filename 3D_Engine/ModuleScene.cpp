#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "Quadtree.h"
#include "Resource.h"
#include "ModuleEditorCamera.h"
#include "Config.h"

#include "ModuleInput.h"
#include "ModuleGui.h"
#include "UIPanelScene.h"

#include "mmgr/mmgr.h"

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
	AABB rootAABB;
	rootAABB.SetNegativeInfinity();
	rootQuadTree = new Quadtree(rootAABB, 0);

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	bool ret = true;
	root->CalculateAllGlobalMatrix();

	if (root->childrens.empty() == false) {
		for (int i = 0; i < root->childrens.size(); i++) {
			ret &= root->childrens[i]->PreUpdate();
		}
	}

	if (gObjSelected)
		UpdateGuizmoOp();

	update_status retUS;
	ret ? retUS = UPDATE_CONTINUE : retUS = UPDATE_ERROR;
	return retUS;
}

update_status ModuleScene::Update(float dt) {

	bool ret = true;

	if (drawRay && App->renderer3D->GetRay())
		DebugDrawLine(line);

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->gui->isMouseOnScene() && !ImGuizmo::IsOver())
		MousePicking();
	if (root->childrens.empty() == false) {

		for (int i = 0; i < root->childrens.size(); i++) {
			ret &= root->childrens[i]->Update();
		}
	}
	update_status retUS;
	ret ? retUS = UPDATE_CONTINUE : retUS = UPDATE_ERROR;
	return retUS;
}

update_status ModuleScene::PostUpdate(float dt) {

	bool ret = true;
	if (root->childrens.empty() == false) {
		for (int i = 0; i < root->childrens.size(); i++) {
			ret &= root->childrens[i]->PostUpdate();
		}
	}
	if (objectMoved) {
		SetQuadTree();
		objectMoved = false;
	}

	update_status retUS;
	ret ? retUS = UPDATE_CONTINUE : retUS = UPDATE_ERROR;
	return retUS;
}

bool ModuleScene::CleanUp()
{

	ClearScene();
	root->CleanUp();
	RELEASE(root);
	RELEASE(rootQuadTree);

	return true;
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

void ModuleScene::SetMainCamera(GameObject * camera)
{
	mainCamera = camera;
}

uuid ModuleScene::GetRandomUUID()
{
	return pcg32_random_r(&rng);
}

void ModuleScene::ClearScene() const
{
	OWN_LOG("Clearing scene");
	for (int i = root->childrens.size() - 1; i > 0; i--) {
		if (root->childrens[i] == mainCamera)
			continue;
	
		root->RemoveChildren(root->childrens[i]);
	}
	for (int i = root->components.size() - 1; i > 0; i--) {
		root->RemoveComponent(root->components[i]);
	}
	App->scene->DeselectAll();
	rootQuadTree->Clear();
}

void ModuleScene::ClearSceneCompletely() 
{
	OWN_LOG("Clearing scene");
	for (int i = root->childrens.size() - 1; i >= 0; i--) {
		root->RemoveChildren(root->childrens[i]);
	}
	for (int i = root->components.size() - 1; i >= 0; i--) {
		root->RemoveComponent(root->components[i]);
	}
	mainCamera = nullptr;
	App->scene->DeselectAll();
	rootQuadTree->Clear();
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

	ClearSceneCompletely();

	char* buffer = nullptr;
	const char* fileName = nullptr;
	file == nullptr ? fileName = SCENE_FILE : fileName = file;
	uint size = App->fileSys->readFile(fileName, &buffer);
	

	if (size < 0) {
		OWN_LOG("Error loading file %s. All data not loaded.", fileName)
		fileName = nullptr;
		RELEASE_ARRAY(buffer);
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
	OWN_LOG("Loading new scene.");
	RELEASE_ARRAY(buffer);

}

void ModuleScene::SetQuadTree()
{
	staticOBjs.clear();
	rootQuadTree->Clear();
	GetAllStaticGOs(root);

	for (auto it : staticOBjs){
		SetQTSize(it);
	}
	for (auto j : staticOBjs) {
		AddGOtoQuadtree(j);
	}
}

void ModuleScene::MousePicking()
{
	ImVec2 normalized = App->gui->panelScene->GetMouse();

	if (normalized.x > -1 && normalized.x < 1){
		if (normalized.y > -1 && normalized.y < 1){

			
			LineSegment ray;
			if (App->scene->inGame) {
				ray = mainCamera->GetComponentCamera()->GetFrustum().UnProjectLineSegment(normalized.x, normalized.y);
			}
			else
				ray = App->camera->cameraComp->GetFrustum().UnProjectLineSegment(normalized.x, normalized.y);

			GetDynamicGOs(root);
			float distance = 9 * 10 ^ 10;
			GameObject* closest = nullptr;
			drawRay = true;
			line = ray;
			for (auto it : dynamicOBjs)
			{
				bool hit;
				float dist;
				it->RayHits(ray, hit, dist);

				if (hit)
				{
					if (dist < distance)
					{
						distance = dist;
						closest = it;
					}
				}
			}

			if (closest != nullptr)
			{
				DeselectAll();
				ShowGameObjectInspector(closest);
			}
		}
	}
}

void ModuleScene::DrawGuizmo(ImGuizmo::OPERATION operation)
{
	ComponentTransformation* transform = gObjSelected->GetComponentTransform();
	if (transform) {
		ImGuizmo::Enable(true);
		ImVec2 pos = { App->gui->panelScene->positionX, App->gui->panelScene->positionY };
		ImVec2 sceneSize = { App->gui->panelScene->width, App->gui->panelScene->height };

		ImGuizmo::SetRect(pos.x, pos.y, sceneSize.x, sceneSize.y);
		float4x4 transMatr;

		float4x4 viewMatrix, projectionMatrix;
		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)viewMatrix.v);
		glGetFloatv(GL_PROJECTION_MATRIX, (float*)projectionMatrix.v);

		transMatr = transform->getGlobalMatrix().Transposed();

		ImGuizmo::Manipulate((float*)viewMatrix.v, (float*)projectionMatrix.v, operation, ImGuizmo::WORLD, transMatr.ptr());
		transMatr.Transpose();

		if (ImGuizmo::IsUsing()) {
			transform->setGlobalMatrix(transMatr);
			root->CalculateAllGlobalMatrix();
			objectMoved = true;
		}
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

void ModuleScene::GetDynamicGOs(GameObject * go)
{
	if (go != root && !go->staticGO)
		dynamicOBjs.push_back(go);
	else if (go == root)
		dynamicOBjs.clear();
	for (auto it : go->childrens) {
		GetDynamicGOs(it);
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

void ModuleScene::UpdateGuizmoOp() {
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		guizmoOp = ImGuizmo::TRANSLATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
	{
		guizmoOp = ImGuizmo::ROTATE;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	{
		guizmoOp = ImGuizmo::SCALE;
	}
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


void ModuleScene::ShowGameObjectInspector(GameObject* newSelected)
{
	if (gObjSelected == newSelected)
		return;
	newSelected->ToggleSelected();
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

	if (gObjSelected != nullptr)
		gObjSelected->ToggleSelected();
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

		//Frustum Culling 
		if (App->camera->GetFrustumCulling())
		{
			if (App->camera->ContainsAaBox(mesh->myGO->globalAABB) != IS_OUT)
				if (mesh->HasMesh()) { mesh->Draw(); }
		}
		else
			if (mesh->HasMesh()) {

				mesh->Draw();
			}
	}

	if (!inGame) {
		if (mainCamera != nullptr) {
			mainCamera->GetComponentCamera()->DebugDraw();
		}
		if (rootQuadTree != nullptr) {
			if (App->renderer3D->GetQuadTree())
				rootQuadTree->DebugDraw();
		}
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



