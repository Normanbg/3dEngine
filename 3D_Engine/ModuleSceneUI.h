#ifndef __MODULE_SCENEUI_H__
#define __MODULE_SCENEUI_H__


#include "Application.h"
#include "Module.h"
#include "Math.h"
#include "RandomGenerator/pcg_variants.h"
#include "RandomGenerator/extras/entropy.h"

class vector;
class GameObject;

class ModuleSceneUI
{
public:
	ModuleSceneUI();
	~ModuleSceneUI();

	void DrawInGameUI();

	void AddUIGameObject(GameObject* go);

private: 
	std::vector<GameObject*> uiGameObjects;
};

#endif//!__MODULE_SCENEUI_H__

