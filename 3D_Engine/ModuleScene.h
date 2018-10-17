#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__


#include "Application.h"
#include "Module.h"

#include <vector>

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

	GameObject* AddGameObject();

public:
	GameObject* root;
};


#endif __MODULE_SCENE_H__