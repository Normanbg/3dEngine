#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "ModuleScene.h"
#include "Component.h"
#include <string>


class GameObject
{
public:
	GameObject();
	~GameObject();
	void Update();
	Component* CreateComponent(ComponentType type);

public:
	std::string name;
	GameObject* parent;
};


#endif // !__GAMEOBJECT_H__
