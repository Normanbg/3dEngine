#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "ModuleScene.h"
#include "Component.h"
#include <string>
#include <vector>


class GameObject
{
public:
	GameObject();
	~GameObject();
	void Update();
	Component* CreateComponent(ComponentType type);

public:
	std::string					name;
	std::vector <Component*>	components;
	GameObject*					parent;
	std::vector <GameObject*>	childrens;

};


#endif // !__GAMEOBJECT_H__
