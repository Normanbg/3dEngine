#ifndef __COMPONENT_H__
#define __COMPONENT_H__

class GameObject;

enum ComponentType {
	NO_TYPE = 0,

	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA
};
class Component {
public:
	Component() {}
	~Component() {
		delete myGO;
		myGO = nullptr;
	}

	virtual void Enable() { active = true; }
	virtual bool PreUpdate() { return true; }
	virtual bool Update() { return true; }
	virtual bool PostUpdate() { return true; }
	virtual void CleanUp() {return; }
	virtual void Disable() { active = false; }
	

public:
	bool active = false;
	ComponentType type;
	GameObject* myGO; //Warning! Duplication with childs of components
};

#endif // !__COMPONENT_H__