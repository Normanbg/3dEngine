#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"
#include "ModuleAudio.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
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
	LOG("Unloading Intro scene");
	
	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	
	dark_floor.Render();
	/*for (p2List_item<Cube>* item = roads.getFirst(); item; item = item->next)
	{
		item->data.Render();
	}
	for (p2List_item<Blades>* item = blades.getFirst(); item; item = item->next)
	{
		btQuaternion quat = item->data.body_cube2->GetRotation();
		quat = quat.normalized();
		float angle = 2 * acos(quat.w()) * 180 / 3.14;
		float den = sqrt(1 - quat.w() *quat.w());
		item->data.cube2.SetRotation(angle, { quat.x()/den,quat.y()/den,quat.z()/den });		
		item->data.cube2.SetPos(item->data.body_cube2->GetPos().x(), item->data.body_cube2->GetPos().y(), item->data.body_cube2->GetPos().z());
		item->data.cube2.Render();		
	}*/

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	/*if (body1->type == Turbo)
	{
		App->audio->PlayFx(turbo_fx);
		App->player->turbo = true;
	}
	else if (body1->type == Floor)
	{
		App->player->RespawnCar();		
	}
	else if (body1->type == LapSensor)
	{
		App->player->half_lap_done = true;
	}
	else if (body1->type == Goal)
	{
		App->player->LapCompleted();
	}
	else if (body1->type == Blade)
	{
		App->player->Hit();
	}*/
}

void ModuleSceneIntro::CreateNormalFloor(float width, float height, float large, float x, float y, float z, Color color)
{
	Cube ret(width, height, large);
	ret.SetPos(x, y, z);
	ret.color = color;

	roads.add(ret);

	App->physics->AddBody(ret, 0);
}

void ModuleSceneIntro::CreateFloorAndWalls(float width, float height, float large, float x, float y, float z, bool wallLeft, bool wallRight, bool wallTop, bool wallBack, Color color)
{
	Cube ret(width, height, large);
	ret.SetPos(x, y, z);
	ret.color = color;

	roads.add(ret);

	if (wallLeft)
	{
		if (width > large)
		{
			Cube LeftWall(width, 3, 1);
			LeftWall.SetPos(x, y + 1.5f, (z -  large / 2) );
			LeftWall.color = color;
			roads.add(LeftWall);
			App->physics->AddBody(LeftWall, 0);
		}

		else if (width < large)
		{
			Cube LeftWall(1, 3, large);
			LeftWall.SetPos((x - width / 2), y + 1.5f, z);
			LeftWall.color = color;
			roads.add(LeftWall);
			App->physics->AddBody(LeftWall, 0);
		}
	}

	if (wallRight)
	{
		if (width > large)
		{
			Cube RightWall(width, 3, 1);
			RightWall.SetPos(x, y + 1.5f, (z + large / 2));
			RightWall.color = color;
			roads.add(RightWall);
			App->physics->AddBody(RightWall, 0);
		}

		else if (width < large)
		{
			Cube RightWall(1, 3, large);
			RightWall.SetPos((x + width / 2), y + 1.5f, z);
			RightWall.color = color;
			roads.add(RightWall);
			App->physics->AddBody(RightWall, 0);
		}
	}

	if (wallTop)
	{
		if (width > large)
		{
			Cube TopWall(1, 3, large);
			TopWall.SetPos((x + width / 2), y + 1.5f, z);
			TopWall.color = color;
			roads.add(TopWall);
			App->physics->AddBody(TopWall, 0);
		}

		else if (width < large)
		{
			Cube TopWall(width, 3, 1);
			TopWall.SetPos(x, y + 1.5f, (z + large / 2));
			TopWall.color = color;
			roads.add(TopWall);
			App->physics->AddBody(TopWall, 0);
		}

	}

	if (wallBack)
	{
		if (width > large)
		{
			Cube BackWall(1, 3, large);
			BackWall.SetPos((x - width / 2), y + 1.5f, z);
			BackWall.color = color;
			roads.add(BackWall);
			App->physics->AddBody(BackWall, 0);
		}

		else if (width < large)
		{
			Cube BackWall(width, 3, 1);
			BackWall.SetPos(x, y + 1.5f, (z - large / 2));
			BackWall.color = color;
			roads.add(BackWall);
			App->physics->AddBody(BackWall, 0);
		}
	}
	App->physics->AddBody(ret, 0);
}

void ModuleSceneIntro::CreateEndFloor(float width, float height, float large, float x, float y, float z, bool wallLeft, bool wallRight)
{
	if(width < large)
		for (int i = 1; i < 6; i++)
		{
			Cube ret(width, height, large / 5);
			ret.SetPos(x, y, (z + (large / 5 * i) - large / 2 - (large / 10)));
			if (i == 2 || i == 4)
				ret.color = White;
			else
				ret.color = Black;
			if (wallLeft)
			{
				Cube LeftWall(width, 3, 1);
				LeftWall.SetPos(x, y + 1.5f, (z - large / 2));
				LeftWall.color = ROAD_COLOR;
				roads.add(LeftWall);
				App->physics->AddBody(LeftWall, 0);
			}
			if (wallRight)
			{
				Cube RightWall(width, 3, 1);
				RightWall.SetPos(x, y + 1.5f, (z + large / 2));
				RightWall.color = ROAD_COLOR;
				roads.add(RightWall);
				App->physics->AddBody(RightWall, 0);
			}
			
			roads.add(ret);
			App->physics->AddBody(ret, 0);
			
		}
	else if (width > large)
	{
		for (int i = 1; i < 6; i++)
		{
			Cube ret(width / 5, height, large);
			ret.SetPos((x + (width / 5 * i) - width / 2 - width /10 ), y, z);
			if (i == 2 || i == 4)
				ret.color = White;
			else
				ret.color = Black;
			if (wallLeft)
			{
				Cube LeftWall(1, 3, large);
				LeftWall.SetPos((x - width / 2), y + 1.5f, z);
				LeftWall.color = ROAD_COLOR;
				roads.add(LeftWall);
				App->physics->AddBody(LeftWall, 0);
			}
			if (wallRight)
			{
				Cube RightWall(1, 3, large);
				RightWall.SetPos((x + width / 2), y + 1.5f, z);
				RightWall.color = ROAD_COLOR;
				roads.add(RightWall);
				App->physics->AddBody(RightWall, 0);
			}
			roads.add(ret);
			App->physics->AddBody(ret, 0);
			
		}
	}

}

void ModuleSceneIntro::CreateTurboPart(float width, float height, float large, float x, float y, float z, Color color)
{
	Cube turbo(width, height, large);
	turbo.SetPos(x, y, z);
	turbo.color = color;

	roads.add(turbo);
	Cube turbosensor = turbo;
	turbosensor.SetPos(x, y + 1.5f, z);

	PhysBody3D* pbody = App->physics->AddBody(turbosensor, 0, SceneObjectType::Turbo);
	pbody->SetSensor();
	pbody->collision_listeners.add(this);
}

void ModuleSceneIntro::CreateRamp(float width, float height, float large, float x, float y, float z, float degree, vec3 axis, Color color) {
	Cube ret(width, height, large);
	ret.SetPos(x, y, z);
	ret.color = color;
	ret.SetRotation(degree, axis);

	roads.add(ret);

	App->physics->AddBody(ret, 0);
}

void ModuleSceneIntro::CreateLowerLimit(float width, float height, float large, float x, float y, float z, Color color) {
	Cube ret(width, height, large);
	ret.SetPos(x, y, z);
	ret.color = color;
	

	roads.add(ret);

	PhysBody3D* pbody = App->physics->AddBody(ret, 0, SceneObjectType::Floor);
	pbody->SetSensor();
	pbody->collision_listeners.add(this);
}

void ModuleSceneIntro::CreateLapSensor(float x, float y, float z, bool isgoal) {

	Cube ret(0.3f, 20,20);
	ret.SetPos(x, y, z);

	SceneObjectType type;

	if (isgoal) {
		type = Goal;
	}
	else {
		type = LapSensor;
	}
	PhysBody3D* pbody = App->physics->AddBody(ret, 0, type);

	pbody->SetSensor();
	pbody->collision_listeners.add(this);
}

void ModuleSceneIntro::CreateBlades(float x, float y, float z, Color color) {

	Cube c(1,1, 1);
	c.SetPos(x, y, z);
	PhysBody3D* c_body = App->physics->AddBody(c,0);

	Cube c2(0.1f, 9.5f, 1);
	c2.SetPos(x + 5, y, z);
	PhysBody3D* c2_body = App->physics->AddBody(c2, 50, SceneObjectType::Blade);
	c2.color = Red;
	c2_body->SetSensor();
	c2_body->collision_listeners.add(this);
	

	App->physics->AddConstraintHinge(*c_body, *c2_body, { 0,0,0 }, { 0,8,0 }, { 0,0,1 }, { 1,0,0 }, true);
	
	Blades bl(c, c2, c_body, c2_body);
	blades.add(bl);
}

void ModuleSceneIntro::StartTerrain()
{
	//CreateRamp(20, ROAD_HEIGHT, 149.6f, 0, 13.20f, 0.4f, 2.5f, {1,0,0}, ROAD_COLOR);//1
	//CreateFloorAndWalls(125, ROAD_HEIGHT, 20, 77.5f, 10, 85, false, true, true);//2
	//CreateFloorAndWalls(19.99f, ROAD_HEIGHT, 20, 0.0f, 10, 85, true, false, true);//3
	//CreateFloorAndWalls(25, ROAD_HEIGHT, 75, 127.5f, 10, 37.5f, true);//4
	//CreateFloorAndWalls(75, ROAD_HEIGHT, 20, 102.5f, 10, -10, false, false, false, true);//5
	//CreateFloorAndWalls(25, ROAD_HEIGHT, 37.5f, 77.5f, 10, -38.75f, true);//6
	//CreateFloorAndWalls(130, ROAD_HEIGHT, 20, 130, 10, -67.5f, true, false, false, true);//7
	//CreateFloorAndWalls(20, ROAD_HEIGHT, 57.5f, 205, 10, -86.25f, false, true);//8
	//CreateFloorAndWalls(112.5f, ROAD_HEIGHT, 20, 158.75f, 10, -125, false, false, true, false);//9
	//CreateNormalFloor(75, ROAD_HEIGHT, 7.5f, 65, 10, -117.5f);//10
	//CreateNormalFloor(75, ROAD_HEIGHT, 7.5f, 65, 10, -132.5f);//11
	//CreateFloorAndWalls(37.5f, ROAD_HEIGHT, 20, 8.75f, 10, -125, false, false, false,true);//12
	//CreateRamp(20, ROAD_HEIGHT, 42.2f, 0, 13.2f, -94.8f ,9, { -1,0,0 }, ROAD_COLOR);//13

	//CreateEndFloor(5, ROAD_HEIGHT, 20, 12.5f, 10, 85, false, true);

	//CreateRamp(20, ROAD_HEIGHT, 10, 130, 10.5, -24,5, { 1,0,0 });//Mini ramp to jump
	//CreateRamp(10, ROAD_HEIGHT, 7.5f, 98, 10.5, -125, 8, { 0,0,-1 });//Mini ramp to jump	

	//CreateTurboPart(5, ROAD_HEIGHT, 20, 65, 10.2f, 85);
	//CreateTurboPart(5, ROAD_HEIGHT, 20, 155, 10.2f, -125);

	//CreateLapSensor( 155, 10, -120.5f);
	//CreateLapSensor(12.5f, 20, 85, true);

	//CreateLowerLimit(1000, 1, 1000, 0, 4.5f, 0);	
	//CreateBlades(75,23,-45);
	//CreateBlades(75, 23, -30);
}

