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

class Primitives
{

public:

	GameObject* CreateSphere();
};


class GameObject
{
	friend ModuleScene;
public:
	GameObject(const char * name);
	~GameObject();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	void CleanUp();

	
	Component* AddComponent(ComponentType type);
	void AddChildren(GameObject* child);


	void GetComponents(ComponentType type, std::vector<Component*>& components);	
	void SetParent(GameObject* parent);
	
	ComponentTransformation* GetTransformComponent();
	ComponentCamera* GetComponentCamera();
	ComponentMaterial * GetComponentMaterial();
	ComponentMesh * GetComponentMesh();

	void CalculateAllGlobalMatrix();

public:
	std::string					name;
	std::vector <Component*>	components;
	GameObject*					parent = nullptr;
	std::vector <GameObject*>	childrens;
	ComponentTransformation*	transformComp;

	bool active = true;
	bool staticGO = false;

private:	
		
	void setName(char* _name);
	void ToggleSelected();


private:
	bool inspectorSelected = false;
};


#endif // !__GAMEOBJECT_H__

