#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Application.h"

#include <vector>

class GameObject;

class ModuleScene {
public:
	GameObject* CreateGameObject();

public:
	GameObject* root;
};


#endif __MODULE_SCENE_H__