#pragma once

class GameObject;

enum ComponentType {
	NO_TYPE = 0,

	TRANSFORM,
	MESH,
	MATERIAL
};
class Component {
public:
	virtual void Enable();
	virtual void Update();
	virtual void Disable();

public:
	bool active = false;
	ComponentType type;
	GameObject* myGo; //Warning! Duplication with childs of components
};