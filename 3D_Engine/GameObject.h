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
class ModuleScene;
class ModuleTextures;


using namespace std;



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
	bool GetSelected();
	void SetParent(GameObject* parent);
	
	ComponentTransformation* GetTransformComponent();
	ComponentCamera* GetComponentCamera();
	ComponentMaterial * GetComponentMaterial();
	ComponentMesh * GetComponentMesh();

	void DrawMeshes();
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

	bool isSelected();
	void setName(char* _name);
	void ToggleSelected();


private:
	bool selected = false;
};


#endif // !__GAMEOBJECT_H__

