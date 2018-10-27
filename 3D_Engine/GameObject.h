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


using namespace std;



class GameObject
{
public:
	GameObject(const char * name);
	~GameObject();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	void CleanUp();

	
	Component* AddComponent(ComponentType type);
	void GetComponents(ComponentType type, std::vector<Component*>& components);

	ComponentTransformation* GetTransformComponent();
	ComponentCamera* GetComponentCamera();

	void DrawMeshes();
	void CalculateAllGlobalMatrix();

private:	
	
	
	
	bool isSelected();
	void setName(char* _name);
	void ToggleSelected();


public:
	std::string					name;
	std::vector <Component*>	components;
	GameObject*					parent = nullptr;
	std::vector <GameObject*>	childrens;
	ComponentTransformation*	transformComp;

private:
	bool selected = false;
};


#endif // !__GAMEOBJECT_H__

