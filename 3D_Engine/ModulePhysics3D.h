#pragma once
#include "Module.h"
#include "Globals.h"

#include "./MathGeoLib/Math/MathAll.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(bool start_enabled = true);
	~ModulePhysics3D();

	bool Init(JSON_Object* obj);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	/*void AddBody(const math::Sphere& sph);
	void AddBody(const math::Capsule& caps);
	void AddBody(const math::Plane& plane);*/
	/*void AddBody(const math::AABB& aabb);*/
/*
	void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, bool disable_collision = false);
*/

	bool isIntersecting();
private:

	bool debug;
	
	/*std::list<math::Sphere> spheres;
	std::list<math::Capsule> capsules;
	std::list<math::Plane> planes;*/
	//std::list<math::AABB> aabbs;
	
	
};
