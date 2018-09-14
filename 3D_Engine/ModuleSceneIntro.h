#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2
#define ROAD_COLOR Grey
#define TURBO_COLOR Orange
#define RAMP_COLOR Green
#define BLADE_COLOR Red
#define LIMIT_COLOR Cyan
#define ROAD_HEIGHT 0.1f

#define LAPS 3

struct PhysBody3D;
struct PhysMotor3D;

struct Blades {

	Blades() {}
	Blades(Cube c, Cube c2, PhysBody3D* bc, PhysBody3D* bc2) : cube(c), cube2(c2), body_cube(bc), body_cube2(bc2) {}
		
	Cube cube;
	Cube cube2;
	PhysBody3D* body_cube = nullptr;
	PhysBody3D* body_cube2 = nullptr;

};


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void StartTerrain();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void CreateNormalFloor(float width, float height, float large, float x, float y, float z, Color color = ROAD_COLOR);
	void CreateFloorAndWalls(float width, float height, float large, float x, float y, float z, bool wallLeft = false, bool wallRight = false, bool wallTop = false, bool wallBack = false, Color color = ROAD_COLOR);
	void CreateEndFloor(float width, float height, float large, float x, float y, float z, bool wallLeft = false, bool wallRight = false);
	void CreateTurboPart(float width, float height, float large, float x, float y, float z, Color color = TURBO_COLOR);
	void CreateRamp(float width, float height, float large, float x, float y, float z,float degree,  vec3 axis, Color color = RAMP_COLOR);
	void CreateLowerLimit(float width, float height, float large, float x, float y, float z, Color color = LIMIT_COLOR);
	void CreateLapSensor(float x, float y, float z, bool isgoal = false);
	void CreateBlades(float x, float y, float z, Color color = BLADE_COLOR);

public:
	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	Cube dark_floor;
	p2List<Cube> roads;
	p2List<Blades> blades;

	uint turbo_fx = 0;
	
};
