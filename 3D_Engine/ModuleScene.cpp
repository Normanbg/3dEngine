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
#include <list>

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
	root->AddComponent(TRANSFORM);
	mainCamera = AddGameObject("Main Camera");
	mainCamera->AddComponent(CAMERA);
	mainCamera->GetComponentCamera()->SetFarPlaneDistance(85.f);
	mainCamera->GetComponentCamera()->LookAt({ 0.f, 0.f, 0.f });
	AABB rootAABB;
	rootAABB.SetNegativeInfinity();
	rootQuadTree = new Quadtree(rootAABB, 0);

	ImGuizmo::Enable(false);

	return true;
}

update_status ModuleScene::PreUpdate(float dt)
{
	bool ret = true;
	root->CalculateAllTransformGlobalMat();
	
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
	GetAllDynamicGOs(root);
	for (auto it : dynamicOBjs) {
		ComponentMesh* mesh = it->GetComponentMesh();
		if (mesh) {
			mesh->frustumContained = ContainsAaBox(mesh->bbox);
		}
	}

	if (rootQuadTree->quadTreeBox.IsFinite()) {
		std::list<uuid> uuidList;
		if (inGame)
			rootQuadTree->Intersects(uuidList, mainCamera->GetComponentCamera()->GetFrustum());
		else
			rootQuadTree->Intersects(uuidList, App->camera->cameraComp->GetFrustum());
		uuidList.sort();
		uuidList.unique();

		intersections = uuidList.size();

		for (auto it : uuidList) {
			ComponentMesh* mesh = GetGameObjectByUUID(it)->GetComponentMesh();
			if (mesh) {
				mesh->frustumContained = ContainsAaBox(mesh->bbox);
			}
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

	GameObject* canvas = GetFirstGameObjectCanvas();
	
	if (canvas) {
		ComponentRectTransform* rect = canvas->GetComponentRectTransform();
		if (rect != nullptr)
			canvas->CalculateAllRectGlobalMat();
	}

	update_status retUS;
	ret ? retUS = UPDATE_CONTINUE : retUS = UPDATE_ERROR;
	return retUS;
}

bool ModuleScene::CleanUp()
{
	
	ClearSceneCompletely();
	root->CleanUp();
	RELEASE(root);
	rootQuadTree->Clear();
	RELEASE(rootQuadTree);

	return true;
}

void ModuleScene::ReceiveEvent(const Event &event)
{
	root->ReceiveEvent(event);
}

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

GameObject * ModuleScene::GetFirstGameObjectCanvas() const
{
	for (int i = 0; i < root->childrens.size(); i++) {
		if (root->childrens[i]->GetComponentCanvas() != nullptr)
			return root->childrens[i];
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

			GetAllGOs(root);
			float distance = 9 * 10 ^ 10;
			GameObject* closest = nullptr;
			drawRay = true;
			line = ray;
			for (auto it : allGameObjects)
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
		ImGuizmo::MODE mode = ImGuizmo::LOCAL;

		float4x4 viewMatrix, projectionMatrix;
		glGetFloatv(GL_MODELVIEW_MATRIX, (float*)viewMatrix.v);
		glGetFloatv(GL_PROJECTION_MATRIX, (float*)projectionMatrix.v);

		transMatr = transform->GetGlobalMatrix().Transposed();

		ImGuizmo::Manipulate((float*)viewMatrix.v, (float*)projectionMatrix.v, operation, mode, transMatr.ptr());
		transMatr.Transpose();

		if (ImGuizmo::IsUsing()) {
			transform->setGlobalMatrix(transMatr);
			root->CalculateAllTransformGlobalMat();
			objectMoved = true;
		}
	}
}

FrustumContained ModuleScene::ContainsAaBox(const AABB& refBox) const
{
	float3 vCorner[8];
	int iTotalIn = 0;
	refBox.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array

	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (inGame) {
				if (mainCamera->GetComponentCamera()->GetFrustum().GetPlane(p).IsOnPositiveSide(vCorner[i]))
				{
					iPtIn = 0;
					--iInCount;
				}
			}
			else {
				if (App->camera->cameraComp->GetFrustum().GetPlane(p).IsOnPositiveSide(vCorner[i]))
				{
					iPtIn = 0;
					--iInCount;
				}
			}
		}
		// were all the points outside of plane p?
		if (iInCount == 0)
			return(IS_OUT);
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
		return(IS_IN);
	// we must be partly in then otherwise
	return(INTERSECT);
}

void ModuleScene::ToggleEditorCam()
{
	GameObject* canvasGO = GetFirstGameObjectCanvas();
	if (canvasGO != nullptr) {
		ComponentCanvas* canvas = canvasGO->GetComponentCanvas();
		canvas->editor = !canvas->editor;
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

void ModuleScene::GetAllDynamicGOs(GameObject* go)
{
	if (go != root && !go->staticGO)
		dynamicOBjs.push_back(go);
	for (auto it : go->childrens) {
		GetAllDynamicGOs(it);
	}
}

void ModuleScene::GetAllGOs(GameObject * go)
{
	if (go != root)
		allGameObjects.push_back(go);
	else if (go == root)
		allGameObjects.clear();
	for (auto it : go->childrens) {
		GetAllGOs(it);
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

void ModuleScene::Draw(bool editor) {

	
	std::vector<Component*> components;
	root->GetComponents(MESH, components);
	

	ComponentMesh* mesh = nullptr;
	for (int i = 0; i < components.size(); i++) {
		mesh = (ComponentMesh *)components[i];

		//Frustum Culling 
		if (App->camera->GetFrustumCulling())
		{
			if (components[i]->myGO->staticGO) {
				mesh->frustumContained = ContainsAaBox(mesh->myGO->globalAABB);
				if (mesh->frustumContained != IS_OUT) {
					if (mesh->HasMesh()) { mesh->Draw(); }
				}
			}
			else {
				if (mesh->frustumContained != IS_OUT){
					mesh->Draw();
				}
			}
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

	float3 corners[8];
	ui_render_box.GetCornerPoints(corners);
	DebugDrawBox(corners, Red, 5.f);


	//--------UI
	if (editor)
	{
		std::vector<ComponentUI*> componentsUI; // first draw UI components
		root->GetComponentsUITypeIgnore(componentsUI, TRANSFORMRECT);
		for (int i = 0; i < componentsUI.size(); i++) {
			if (componentsUI[i]->draw)
				componentsUI[i]->DrawUI();
		}
		componentsUI.clear();

		root->GetComponentsUIType(componentsUI, TRANSFORMRECT); // then draw rectTransforms

		ComponentRectTransform* recTrans = nullptr;
		for (int i = 0; i < componentsUI.size(); i++) {
			recTrans = (ComponentRectTransform *)componentsUI[i];
			if (recTrans->draw)
				recTrans->DrawUI();
		}
		componentsUI.clear();
		//--------UI
		recTrans = nullptr;
	}
	mesh = nullptr;
}

void ModuleScene::DrawInGameUI()
{
	GameObject* canvas = GetFirstGameObjectCanvas();
	if (canvas) {
		ComponentRectTransform* rectTransform = canvas->GetComponentRectTransform();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		float left = rectTransform->GetGlobalPos().x;
		float right = rectTransform->GetGlobalPos().x + rectTransform->GetWidth();
		float top = rectTransform->GetGlobalPos().y + rectTransform->GetHeight();
		float bottom = rectTransform->GetGlobalPos().y;
		float zNear = -10.f;
		float zFar = 10.f;

		float3 min = { left, bottom, zNear };
		float3 max = { right, top, zFar };

		ui_render_box.minPoint = min;
		ui_render_box.maxPoint = max;
		
		glOrtho(left, right, bottom, top, zNear, zFar);
		float3 corners[8];
		ui_render_box.GetCornerPoints(corners);
		DebugDrawBox(corners, Red, 5.f);

		std::vector<ComponentUI*> componentsUI; // first draw UI components
		root->GetComponentsUITypeIgnore(componentsUI, TRANSFORMRECT);
		for (int i = 0; i < componentsUI.size(); i++) {
			if (componentsUI[i]->draw)
				componentsUI[i]->DrawUI();
		}
		componentsUI.clear();

	}
}

GameObject * ModuleScene::AddGameObject()
{
	GameObject* ret = new GameObject();
	ret->parent = root;
	root->childrens.push_back(ret);
	ret->AddComponent(TRANSFORM);
	return ret;
}


GameObject* ModuleScene::AddGameObject(const char* name) {
	GameObject* ret = new GameObject(name);
	ret->parent = root;
	root->childrens.push_back(ret);
	ret->AddComponent(TRANSFORM);
	return ret;
}

GameObject * ModuleScene::AddGameObject(const char * name, GameObject * parent)
{
	GameObject* ret = new GameObject(name);
	ret->parent = parent;
	parent->childrens.push_back(ret);
	ret->AddComponent(TRANSFORM);
	return ret;
}

GameObject * ModuleScene::AddUIGameObject(const char * name, GameObject * parent)
{
	GameObject* ret = new GameObject(name);
	ret->parent = parent;
	parent->childrens.push_back(ret);
	ret->AddUIComponent(TRANSFORMRECT);
	uiGameObjects.push_back(ret);
	return ret;
}

