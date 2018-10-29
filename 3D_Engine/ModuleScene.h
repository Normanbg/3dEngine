#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__


#include "Application.h"
#include "Module.h"

class vector;
class GameObject;

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

	void AddGameObjectToSelectedList(GameObject * newSelected);
	void DeselectAll();

	void SetBoundingBox(bool active);
	void SetWireframe(bool active);

	void DrawMeshes();
	GameObject* AddGameObject(const char* name);
	GameObject* AddGameObject(const char* name, GameObject* parent);
	GameObject* CreateCube();

public:
	GameObject* root;
	std::vector<GameObject*> gObjsSelected;
	int numCubes = 0;
};


#endif __MODULE_SCENE_H__