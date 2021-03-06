#ifndef __MODULE_EDITOR_CAM_H__
#define __MODULE_EDITOR_CAM_H__

#include "Module.h"
#include "Math.h"
#include "Globals.h"

class GameObject;
class ComponentCamera;
class Quadtree;
class list;

class ModuleEditorCamera : public Module
{
public:
	ModuleEditorCamera(bool start_enabled = true);
	~ModuleEditorCamera();

	bool Init(JSON_Object* data) override;
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//FrustumContained ContainsAaBox(const AABB & refBox) const;
	//void QTContainsAaBox(Quadtree* qt);

	void ConfigInfo();

	const float GetMouseSensit() const;
	const float GetScrollSensit() const;
	const bool GetFrustumCulling() const;

public:
	ComponentCamera* cameraComp;

private:
	void MouseMovement(float dt);
	void Orbit(float dt);

private:
	float mouseSensitivity = 0.25f;
	float scrollWheelSensitivity = 10.0f;
	bool frustumCulling = true;
};

#endif //__MODULE_EDITOR_CAM_H__