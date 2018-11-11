#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__


#include "Application.h"
#include "Module.h"
#include "Math.h"
#include "RandomGenerator/pcg_variants.h"
#include "RandomGenerator/extras/entropy.h"

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
	uint GetRandomUUID();

	void ClearScene()const;
	void SaveScene(const char* file = nullptr);
	void LoadScene(const char* file= nullptr);

	//void SetQuadTree();
	//void AddGOtoQuadtree(GameObject* go) ;

public:

	GameObject* root;
	GameObject* mainCamera = nullptr;
	GameObject* gObjSelected = nullptr;
	Material* materialSelected = nullptr;
	
	bool inGame = false;
	
	int numCubes = 0;	

	uint staticsGObjs = 0;
	Quadtree* rootQuadTree = nullptr;

private:
	void GetAllGObjs(GameObject* go);

private:
	pcg32_random_t rng;
	std::vector<GameObject*> gObjs;


	
};
#endif __MODULE_SCENE_H__