
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "PhysBody3D.h"





ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	OWN_LOG("Loading Intro assets");
	bool ret = true;

	

	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(vec3(0, 0, 0));
	//App->audio->PlayMusic("audio/music.ogg");
	//turbo_fx = App->audio->LoadFx("audio/turbo.wav");
	//StartTerrain();
	
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
	math::Plane(vec(0, 1, 0), 1);
	/*math::Line(vec(0, 0, 0), vec(1, 2, 3));*/
	math::float3 vectorTest(0.0f, 0.0f, 0.0f);
	//vectorTest = { 0, 0, 0 };
	math::Sphere sp(vectorTest, 20);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2){
}
