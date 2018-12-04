#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "ModuleScene.h"
#include "Config.h"

class GameObject;


enum ComponentType {
	NO_TYPE = 0,

	TRANSFORM,
	RECT_TRANSFORM,
	CAMERA,
	MESH,
	MATERIAL
};

class Component {
public:
	Component() { 
		UUID = App->scene->GetRandomUUID();
	}
	~Component() {	}

	virtual void Enable() { active = true; }
	virtual bool PreUpdate() { return true; }
	virtual bool Update() { return true; }
	virtual bool PostUpdate() { return true; }
	virtual void CleanUp() {return; }
	virtual void Disable() { active = false; }
	
	virtual void DrawInspector() { return; };

	inline const uint GetUUID() const { return UUID; }


	virtual void Save(Config& data)const {};
	virtual void Load(Config* data) {};

public:
	bool active = false;
	ComponentType type;
	GameObject* myGO = nullptr; //Warning! Duplication with childs of components


protected:

	uuid UUID= 0;

	
};

#endif // !__COMPONENT_H__