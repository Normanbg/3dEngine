#include "ModuleTime.h"
#include "ModuleScene.h"

ModuleTime::ModuleTime(bool start_enabled) : Module(start_enabled) {
	
}


ModuleTime::~ModuleTime()
{
}

update_status ModuleTime::PreUpdate(float dt)
{
	if (App->scene->inGame) {
		gameDeltaTime = dt * timeScale;
		gameClock += gameDeltaTime;
	}
	return UPDATE_CONTINUE;
}

bool ModuleTime::Start()
{
	realTimeClock.Stop();
	realTimeClock.SetZero();
	
	return true;
}

void ModuleTime::Reset()
{
	realTimeClock.Resume();
}

void ModuleTime::Play()
{
	realTimeClock.Start();
	
}

void ModuleTime::Resume()
{
	realTimeClock.Resume();
	paused = false;
}

void ModuleTime::Pause()
{
	realTimeClock.Stop();
	paused = true;
}

