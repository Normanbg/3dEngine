#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__


#include "Application.h"
#include "Module.h"
#include "Math.h"
#include "RandomGenerator/pcg_variants.h"
#include "RandomGenerator/extras/entropy.h"
#include "ModuleSceneUI.h"

class vector;
class GameObject;
class Quadtree;
class Resource;



class ModuleScene : public Module
{
	friend class ModuleSceneUI;

public:

	ModuleScene(bool start_enabled = true);
	~ModuleScene();


	bool Init(JSON_Object* obj) override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void ShowGameObjectInspector(GameObject * newSelected);
	void ShowTextureResourceInspector(uuid newSelected);
	void DeselectAll();

	void SetBoundingBox(bool active);
	void SetWireframe(bool active);


	void Draw(bool editor);
	void DrawInGameUI();

	GameObject* AddGameObject();
	GameObject* AddGameObject(const char* name);	
	GameObject* AddGameObject(const char* name, GameObject* parent);
	GameObject * AddUIGameObject(const char * name, GameObject * parent);

	GameObject* GetGameObjectByUUID(uuid UUID) const;
	GameObject* GetFirstGameObjectCanvas() const;

	void SetMainCamera(GameObject* camera);
	void ChangeRootGO(GameObject* newRoot){ root = newRoot; }
	uuid GetRandomUUID();

	void ClearScene()const;
	void ClearSceneCompletely() ;
	void SaveScene(const char* file = nullptr);
	void LoadScene(const char* file= nullptr);
	
	void SetQuadTree();
	void MousePicking();

	void DrawGuizmo(ImGuizmo::OPERATION operation);
	FrustumContained ContainsAaBox(const AABB & refBox) const;

	void ToggleEditorCam();

public:

	GameObject* root;
	GameObject* mainCamera = nullptr;
	GameObject* gObjSelected = nullptr;
	uuid TextureResourceSelected = 0;
	
	bool inGame = false;
	bool drawRay = false;
	bool objectMoved = false;

	int numCubes = 0;	

	char currentScene[64] = "NewScene";

	uint staticsGObjs = 0;
	Quadtree* rootQuadTree = nullptr;

	LineSegment line;

	ImGuizmo::OPERATION guizmoOp = ImGuizmo::TRANSLATE;

	//--------------------
	std::list<GameObject*> staticObjsToDraw;

	uint intersections = 0;
private:
	void GetAllStaticGOs(GameObject* go);
	void GetAllDynamicGOs(GameObject* go);
	void GetAllGOs(GameObject* go);
	void AddGOtoQuadtree(GameObject * go);

	void UpdateGuizmoOp();

	void SetQTSize(GameObject * go);

	GameObject* GetGameObjectUUIDRecursive(uuid UUID, GameObject* go) const;

private:
	pcg32_random_t rng;
	std::vector<GameObject*> staticOBjs;
	std::vector<GameObject*> dynamicOBjs;
	std::vector<GameObject*> allGameObjects;

	std::vector<GameObject*> uiGameObjects;

};
#endif __MODULE_SCENE_H__