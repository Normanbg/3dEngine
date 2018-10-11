#pragma once
#include "Module.h"
#include "Globals.h"

#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Geometry/GeometryAll.h"

#include "RandomGenerator/pcg_variants.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	/*void OnCollision(PhysBody3D* body1, PhysBody3D* body2);*/
public:

};
