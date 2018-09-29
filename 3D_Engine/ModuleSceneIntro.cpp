
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



	// --------------------------Draw Axis Grid
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	glEnd();

	// --------------------------Draw Plane Grid

	glLineWidth(1.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

	// --------------------------








	return UPDATE_CONTINUE;
}

//void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2){
//}
