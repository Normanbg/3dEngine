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
	virtual void Enable() { active = true; }
	virtual bool PreUpdate() { return true; }
	virtual bool Update() { return true; }
	virtual bool PostUpdate() { return true; }
	virtual void Disable() { active = false; }

public:
	bool active = false;
	ComponentType type;
	GameObject* myGO; //Warning! Duplication with childs of components
};