#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include "Globals.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"


#include "Component.h"
#include "ComponentTransformation.h"
#include "ComponentCanvas.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"



class string;
class vector;

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
	
	//DO NOT USE CONST FOR COMPONENTS AS THEY HAVE TO INITIALIZE OTHER COMPONENTS, ONLY FOR THE FUNCTION
	ComponentTransformation* GetComponentTransform() const;
	ComponentCamera* GetComponentCamera() const;
	ComponentMaterial * GetComponentMaterial() const;
	ComponentMesh * GetComponentMesh() const;
	ComponentCanvas * GetComponentCanvas() const;
	ComponentMaterial* GetComponentMaterial(const uuid UUID);

	const bool GetSelected() const;


	inline const uuid GetUUID() { return UUID; }

	void CalculateAllGlobalMatrix();
	void SetChildsStatic(bool active);

	void SetLocalAABB(AABB aabb);

	void RayHits(const LineSegment& segment, bool& hit, float& dist);
public:
	std::string					name;
	std::vector <Component*>	components;
	GameObject*					parent = nullptr;
	std::vector <GameObject*>	childrens;

	AABB localAABB;
	AABB globalAABB;
	OBB obb;

	bool active = true;
	bool staticGO = false;	

private:	
		
	void SetName(char* _name);
	
	void ToggleSelected();
	uuid UUID = 0;

private:
	bool inspectorSelected = false;
};


#endif // !__GAMEOBJECT_H__

