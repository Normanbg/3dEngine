#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleTextures.h"
#include "Component.h"
#include "ComponentTransformation.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"


class string;
class vector;


class GameObject
{
public:
	GameObject(char * name);
	~GameObject();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	void CleanUp();
	void CalculateAllGlobalMatrix();
	Component* AddComponent(ComponentType type);
	ComponentTransformation* GetTransformComponent();
	ComponentCamera* GetComponentCamera();
	void setName(char* _name);

public:
	std::string					name;
	std::vector <Component*>	components;
	GameObject*					parent = nullptr;
	std::vector <GameObject*>	childrens;
	ComponentTransformation*	transformComp;
};


#endif // !__GAMEOBJECT_H__
