#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__


#include "Application.h"
#include "Module.h"
#include "Math.h"

class vector;
class GameObject;
class Material;
class Quadtree;

class ModuleScene : public Module
{
public:

	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Init(JSON_Object* obj) override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void ShowGameObjectInspector(GameObject * newSelected);
	void ShowMaterialInspector(Material * newSelected);
	void DeselectAll();

	void SetBoundingBox(bool active);
	void SetWireframe(bool active);

	void Draw();

	GameObject* AddGameObject();
	GameObject* AddGameObject(const char* name);	
	GameObject* AddGameObject(const char* name, GameObject* parent);
	GameObject* CreateCube();
	GameObject* GetGameObjectByUUID(uint uuid) const;
	GameObject* GetGameObjectUUIDRecursive(uint uuid, GameObject* go) const;

	void ChangeRootGO(GameObject* newRoot){ root = newRoot; }

	void ClearScene()const;
	bool SaveScene()const;
	bool LoadScene(const char* data);

	void AddGOtoQuadtree(GameObject* go);

public:

	GameObject* root;
	GameObject* mainCamera = nullptr;
	GameObject* gObjSelected = nullptr;
	Material* materialSelected = nullptr;

	bool inGame = false;
	
	int numCubes = 0;
	LCG* random = nullptr;//MARGELI WHATS THIS? USE PCG RANDOM NOT MATHGEOLIB PLS!!!!!!!!

	Quadtree* rootQuadTree = nullptr;
};
#endif __MODULE_SCENE_H__