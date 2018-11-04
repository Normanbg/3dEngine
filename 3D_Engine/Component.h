#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "ModuleScene.h"

class GameObject;
struct Config;

enum ComponentType {
	NO_TYPE = 0,

	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA
};
class Component {
public:
	Component() { 
		uuid = App->scene->random->Int(); }
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
	
	virtual void DrawInspector() { return; };

	inline const uint GetUUID() { return uuid; }


	virtual void Save(Config& data)const {};
	virtual void Load(Config* data) {};

public:
	bool active = false;
	ComponentType type;
	GameObject* myGO; //Warning! Duplication with childs of components

private:

	uint uuid= 0;
};

#endif // !__COMPONENT_H__