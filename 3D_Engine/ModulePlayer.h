//#pragma once
//#include "Module.h"
//#include "Globals.h"
//#include "p2Point.h"
//#include "Timer.h"
//
//struct PhysVehicle3D;
//
//#define MAX_ACCELERATION 1000.0f
//#define TURBO_ACCELERATION 8000.0f
//#define TURN_DEGREES 15.0f * DEGTORAD
//#define BRAKE_POWER 75.0f
//
//
//
//class ModulePlayer : public Module
//{
//public:
//	ModulePlayer(Application* app, bool start_enabled = true);
//	virtual ~ModulePlayer();
//
//	bool Start();
//	update_status Update(float dt);
//	bool CleanUp();
//	btVector3 getPos() const;//Get axis of orientation of the player	
//	
//	void InitialPos() const;//Car spawns to the initial pos
//	void RespawnCar() ;
//	void LapCompleted();
//	void Win();
//	void Hit();
//
//public:
//
//	uint brakes_fx = 0;
//	uint complete_lap_fx = 0; 
//	uint last_lap_fx = 0;
//	uint win_fx = 0;
//	uint accelerating_fx = 0;
//	uint blade_hit_fx = 0;
//
//	PhysVehicle3D* vehicle;
//	float turn = 0;
//	float acceleration = 0;
//	float acceleration_fx_Time = 0;
//	float brake = 0;
//	bool turbo = false;
//
//	mutable Timer timer;
//	float best_time= 100;
//	uint current_laps = 1;
//	bool half_lap_done = false;
//	bool input = true;
//};