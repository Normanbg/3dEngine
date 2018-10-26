#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleTextures.h"
#include "Component.h"
#include "ComponentTransformation.h"
#include "ComponentCamera.h"


class string;
class vector;


class GameObject
{
public:
	GameObject();
	~GameObject();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	void CleanUp();
	void CalculateAllGlobalMatrix();
	Component* AddComponent(ComponentType type);
	Component* GetComponent(ComponentType type);


public:
	std::string					name;
	std::vector <Component*>	components;
	GameObject*					parent = nullptr;
	std::vector <GameObject*>	childrens;
	ComponentTransformation*	transformComp;
};


#endif // !__GAMEOBJECT_H__
