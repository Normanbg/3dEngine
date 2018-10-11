
#include "Application.h"
#include "ModuleSceneIntro.h"

#include <time.h>
#include "RandomGenerator/extras/entropy.h"





ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled){
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	OWN_LOG("Loading Intro assets");
	

	bool ret = true;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	OWN_LOG("Unloading Intro scene");
	
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}

//void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2){
//}
