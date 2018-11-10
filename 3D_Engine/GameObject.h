#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include "Globals.h"
#include "ModuleScene.h"
#include "ModuleTextures.h"
#include "ModuleRenderer3D.h"


#include "Component.h"
#include "ComponentTransformation.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"



class string;
class vector;
//class ModuleScene;
//class ModuleTextures;


using namespace std;

struct Config;

class GameObject
{
	friend ModuleScene;
public:
	GameObject();
	GameObject(const char * name);
	~GameObject();


	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	void CleanUp();

	void Save(Config& data)const;
	void Load(Config* data);
	
	Component* AddComponent(ComponentType type);
	void AddComponent(Component* component, ComponentType type);
	void AddChildren(GameObject* child);

	void RemoveChildren(GameObject* toRemove);
	void RemoveComponent(Component* comp);

	void GetComponents(ComponentType type, std::vector<Component*>& components);	
	void SetParent(GameObject* parent);
	
	ComponentTransformation* GetTransformComponent();
	ComponentCamera* GetComponentCamera();
	ComponentMaterial * GetComponentMaterial();
	ComponentMesh * GetComponentMesh();
	ComponentMaterial* GetComponentMaterial(const uint uuid);

	inline const uint GetUUID() { return uuid; }

	void CalculateAllGlobalMatrix();
	void setChildsStatic();

	void SetLocalAABB(AABB aabb);

public:
	std::string					name;
	std::vector <Component*>	components;
	GameObject*					parent = nullptr;
	std::vector <GameObject*>	childrens;
	ComponentTransformation*	transformComp;

	AABB localAABB;
	AABB globalAABB;
	OBB obb;

	bool active = true;
	bool staticGO = false;	

private:	
		
	void setName(char* _name);
	
	void ToggleSelected();
	uint uuid = 0;

private:
	bool inspectorSelected = false;
};


#endif // !__GAMEOBJECT_H__

