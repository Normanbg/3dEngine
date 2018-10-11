#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"


#include <list>

ModulePhysics3D::ModulePhysics3D(bool start_enabled) : Module(start_enabled){
	debug = false;
	name = "Physics";
}

// Destructor
ModulePhysics3D::~ModulePhysics3D(){

}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init(JSON_Object* obj)
{
	OWN_LOG("Creating 3D Physics simulation");
	bool ret = true;

	json_object_clear(obj);//clear obj to free memory
	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	OWN_LOG("Creating Physics environment");

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	OWN_LOG("Destroying 3D Physics simulation");
	
	return true;

}
