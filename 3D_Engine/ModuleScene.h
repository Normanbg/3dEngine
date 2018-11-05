#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__


#include "Application.h"
#include "Module.h"
#include "MathGeoLib/Algorithm/Random/LCG.h"

class vector;
class GameObject;
class Material;

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

	void DrawMeshes();
	GameObject* AddGameObject();
	GameObject* AddGameObject(const char* name);
	
	GameObject* AddGameObject(const char* name, GameObject* parent);
	GameObject* CreateCube();
	GameObject* GetGameObjectByUUID(uint uuid) const;
	void ChangeRootGO(GameObject* newRoot){ root = newRoot; }

	bool SaveScene()const;
	bool LoadScene(const char* data);


public:

	GameObject* GetGameObjectUUIDRecursive(uint uuid, GameObject* go) const;

	GameObject* root;
	GameObject* gObjSelected = nullptr;
	Material* materialSelected = nullptr;
	
	int numCubes = 0;
	LCG* random = nullptr;
};


#endif __MODULE_SCENE_H__