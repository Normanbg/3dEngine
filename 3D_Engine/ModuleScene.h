#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"
#include "Application.h"


#include <vector>

class GameObject;

class ModuleScene : public Module
{
public:

	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Init(JSON_Object* obj);
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void CreateGameObject(GameObject* gameObject);

public:
	GameObject* root;
};


#endif __MODULE_SCENE_H__