//#include "Globals.h"
//#include "Application.h"
//#include "ModulePlayer.h"
//#include "Primitive.h"
//#include "PhysVehicle3D.h"
//#include "PhysBody3D.h"
//#include "ModuleAudio.h"
//
//
//ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
//{
//	turn = acceleration = brake = 0.0f;
//}
//
//ModulePlayer::~ModulePlayer()
//{}
//
// Load assets
//bool ModulePlayer::Start()
//{
//	LOG("Loading player");
//
//	VehicleInfo car;
//
//	// Car properties ----------------------------------------
//	car.chassis_size.Set(2.6f, 1.2f, 7);
//	car.chassis_offset.Set(0,1.6f, 0.5f);
//	car.mass = 450.0f;
//	car.suspensionStiffness = 17.88f;
//	car.suspensionCompression = 0.83f;
//	car.suspensionDamping = 0.88f;
//	car.maxSuspensionTravelCm = 1000.0f;
//	car.frictionSlip = 50.5;
//	car.maxSuspensionForce = 6000.0f;
//
//	// Wheel properties ---------------------------------------
//	float connection_height = 2.0f;
//	float wheel_radius = 0.75f;
//	float wheel_width = 0.65f;
//	float suspensionRestLength = 1.2f;
//
//	// Don't change anything below this line ------------------
//
//	float half_width = car.chassis_size.x*0.5f;
//	float half_length = car.chassis_size.z*0.5f;
//	
//	vec3 direction(0,-1,0);
//	vec3 axis(-1,0,0);
//	
//	car.num_wheels = 4;
//	car.wheels = new Wheel[4];
//
//	// FRONT-LEFT ------------------------
//	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
//	car.wheels[0].direction = direction;
//	car.wheels[0].axis = axis;
//	car.wheels[0].suspensionRestLength = suspensionRestLength;
//	car.wheels[0].radius = wheel_radius;
//	car.wheels[0].width = wheel_width;
//	car.wheels[0].front = true;
//	car.wheels[0].drive = true;
//	car.wheels[0].brake = false;
//	car.wheels[0].steering = true;
//
//	// FRONT-RIGHT ------------------------
//	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
//	car.wheels[1].direction = direction;
//	car.wheels[1].axis = axis;
//	car.wheels[1].suspensionRestLength = suspensionRestLength;
//	car.wheels[1].radius = wheel_radius;
//	car.wheels[1].width = wheel_width;
//	car.wheels[1].front = true;
//	car.wheels[1].drive = true;
//	car.wheels[1].brake = false;
//	car.wheels[1].steering = true;
//
//	// REAR-LEFT ------------------------
//	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
//	car.wheels[2].direction = direction;
//	car.wheels[2].axis = axis;
//	car.wheels[2].suspensionRestLength = suspensionRestLength;
//	car.wheels[2].radius = wheel_radius;
//	car.wheels[2].width = wheel_width;
//	car.wheels[2].front = false;
//	car.wheels[2].drive = false;
//	car.wheels[2].brake = true;
//	car.wheels[2].steering = false;
//
//	// REAR-RIGHT ------------------------
//	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
//	car.wheels[3].direction = direction;
//	car.wheels[3].axis = axis;
//	car.wheels[3].suspensionRestLength = suspensionRestLength;
//	car.wheels[3].radius = wheel_radius;
//	car.wheels[3].width = wheel_width;
//	car.wheels[3].front = false;
//	car.wheels[3].drive = false;
//	car.wheels[3].brake = true;
//	car.wheels[3].steering = false;
//
//	vehicle = App->physics->AddVehicle(car);
//	InitialPos();
//	
//	complete_lap_fx = App->audio->LoadFx("audio/complete_lap.wav");
//	last_lap_fx = App->audio->LoadFx("audio/last_lap.wav");
//	win_fx = App->audio->LoadFx("audio/win.wav");
//	brakes_fx = App->audio->LoadFx("audio/brakes.wav");
//	accelerating_fx = App->audio->LoadFx("audio/accelerating.wav");
//	blade_hit_fx = App->audio->LoadFx("audio/blade_hit.wav");
//
//	timer.Start();
//
//	return true;
//}
//
// Unload assets
//bool ModulePlayer::CleanUp()
//{
//	LOG("Unloading player");
//
//	return true;
//}
//
//btVector3 ModulePlayer::getPos() const
//{
//	return vehicle->GetPos();
//}
//
// Update: draw background
//update_status ModulePlayer::Update(float dt)
//{
//	if (input){
//	turn = acceleration = brake = 0.0f;
//
//		
//	 
//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
//	{
//		acceleration = MAX_ACCELERATION;
//	}
//
//	if (acceleration == MAX_ACCELERATION && SDL_GetTicks() > acceleration_fx_Time)//This function makes the acceleration_fx not sound repetitive
//	{
//		App->audio->PlayFx(accelerating_fx);
//		acceleration_fx_Time = SDL_GetTicks() + 450;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
//	{
//		if (turn < TURN_DEGREES)
//			turn += TURN_DEGREES;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
//	{
//		if (turn > -TURN_DEGREES)
//			turn -= TURN_DEGREES;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
//	{
//		acceleration -= MAX_ACCELERATION / 2;
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
//	{
//		if (vehicle->GetKmh() > 7.5f)
//		{
//			App->audio->PlayFx(brakes_fx);
//			brake = BRAKE_POWER;
//		}
//		else
//			brake = BRAKE_POWER;
//	}
//}
//	else { // Once won, 
//		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
//		{
//			RespawnCar();
//		}
//	}
//	if (turbo) {
//		acceleration += TURBO_ACCELERATION;
//		
//		turbo = false;
//	
//	}
//	vehicle->ApplyEngineForce(acceleration);
//	vehicle->Turn(turn);
//	vehicle->Brake(brake);
//
//	vehicle->Render();
//
//	char title[80];
//	sprintf_s(title, "%i/%i LAPS    %.1f Km/h    %.1f sec      Best Time %.1f sec", current_laps, LAPS, vehicle->GetKmh(), timer.ReadSec(), best_time);
//	App->window->SetTitle(title);
//
//	return UPDATE_CONTINUE;
//}
//
//void ModulePlayer::InitialPos() const {
//
//	vehicle->SetRotation({ 0,1,0,1 });
//	vehicle->SetPos(5, 10.1f, 85);	
//}
//
//void ModulePlayer::RespawnCar()  {
//	InitialPos();
//	vehicle->SetRotation({0,1,0,1 });
//	vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });
//	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0, 0, 0 });
//	timer.Start();
//	current_laps = 1;
//	half_lap_done = false;
//	App->camera->free_camera = false;
//	input = true;
//}
//
//void ModulePlayer::LapCompleted() {
//	if (half_lap_done) {
//		App->audio->PlayFx(complete_lap_fx);
//		current_laps++;
//		half_lap_done = false;
//		if (current_laps == LAPS)
//			App->audio->PlayFx(last_lap_fx);
//
//		if (current_laps > LAPS) 
//		{
//			App->audio->PlayFx(win_fx);
//			Win();		
//		}
//	}
//}
//
//void ModulePlayer::Win() {
//	input = false;
//	App->audio->PlayFx(win_fx);
//	App->camera->free_camera = true;
//	timer.Stop();
//	if (timer.ReadSec() < best_time) {
//		best_time = timer.ReadSec();	
//	}
//}
//
//void ModulePlayer::Hit() {
//	App->audio->PlayFx(blade_hit_fx);
//	RespawnCar();
//}