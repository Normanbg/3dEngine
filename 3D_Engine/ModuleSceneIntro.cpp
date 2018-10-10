
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
	/*glLineWidth(10.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 20.f, 0.f);
	glEnd();
	glLineWidth(1.0f);


	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(.0f, .0f, .0f);
	glVertex3f(1.0f, .0f, .0f);
	glVertex3f(.0f, 1.0f, .0f);
	glEnd();*/


	return UPDATE_CONTINUE;
}

//void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2){
//}
