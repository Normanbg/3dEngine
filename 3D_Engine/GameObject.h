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
#include "ComponentRectTransform.h"
#include "ComponentImageUI.h"
#include "ComponentTextUI.h"
#include "ComponentButtonUI.h"
#include "ComponentInputUI.h"
#include "ComponentWindowUI.h"
#include "ComponentCheckBoxUI.h"

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
	ComponentUI * AddUIComponent(ComponentTypeUI type);
	void AddComponent(Component* component, ComponentType type);
	void AddChildren(GameObject* child);

	void RemoveChildren(GameObject* toRemove);
	void RemoveComponent(Component* comp);
	void RemoveComponentUI(ComponentUI * comp);

	void GetComponents(ComponentType type, std::vector<Component*>& components);
	void GetAllComponentsUI(std::vector<ComponentUI*>& comp);
	void GetComponentsUITypeIgnore( std::vector<ComponentUI*>& comp, ComponentTypeUI ignoreType = NOTYPE, bool recursive = true);
	void GetComponentsUIType(std::vector<ComponentUI*>& comp, ComponentTypeUI type, bool recursive = true);
	void SetParent(GameObject* parent);
	void DoFadeAndDelete();
	

	//DO NOT USE CONST FOR COMPONENTS AS THEY HAVE TO INITIALIZE OTHER COMPONENTS, ONLY FOR THE FUNCTION
	ComponentTransformation* GetComponentTransform() const;
	ComponentCamera* GetComponentCamera() const;
	ComponentMaterial * GetComponentMaterial() const;
	ComponentMaterial* GetComponentMaterial(uuid UUID);
	ComponentMesh * GetComponentMesh() const;
	ComponentCanvas * GetComponentCanvas() const;
	ComponentRectTransform * GetComponentRectTransform() const;

	//UI COMPONENTS------------
	ComponentImageUI * GetComponentImageUI() const;
	ComponentTextUI * GetComponentTextUI() const;
	ComponentInputUI * GetComponentInputUI() const;
	ComponentButtonUI * GetComponentButtonUI() const;
	ComponentWindowUI * GetComponentWindowUI() const;
	ComponentCheckBoxUI * GetComponentCheckBoxUI() const;

	const bool GetSelected() const;


	inline const uuid GetUUID() { return UUID; }

	void CalculateAllTransformGlobalMat();
	void CalculateAllRectGlobalMat();
	void SetChildsStatic(bool active);

	void SetLocalAABB(AABB aabb);

	void RayHits(const LineSegment& segment, bool& hit, float& dist);

public:
	std::string					name;
	std::vector <Component*>	components;
	std::vector <ComponentUI*>	componentsUI;
	GameObject*					parent = nullptr;
	std::vector <GameObject*>	childrens;

	AABB localAABB;
	AABB globalAABB;
	OBB obb;

	bool active = true;
	bool staticGO = false;	

private:	
		
	void SetName(char* _name);
	void ReceiveEvent(const Event &event);
	
	void ToggleSelected();
	uuid UUID = 0;


private:
	bool inspectorSelected = false;
	bool fadeDelete= false;
};


#endif // !__GAMEOBJECT_H__

