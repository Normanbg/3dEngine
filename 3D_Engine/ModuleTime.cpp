#include "ModuleTime.h"
#include "ModuleScene.h"


#include "mmgr/mmgr.h"

ModuleTime::ModuleTime(bool start_enabled) : Module(start_enabled) {
	
}


ModuleTime::~ModuleTime()
{
}

update_status ModuleTime::PreUpdate(float dt)
{
	if (playing) {
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
	realTimeClock.SetZero();
	gameClock = 0;
}

void ModuleTime::Play()
{
	playing = true;
	realTimeClock.Start();	
}

void ModuleTime::Resume()
{
	realTimeClock.Resume();
	playing = true;
	paused = false;
}

void ModuleTime::Pause()
{
	realTimeClock.Stop();
	playing = false;
	paused = true;
}

void ModuleTime::Stop()
{
	realTimeClock.Stop();
	realTimeClock.SetZero();
	playing = false;
	gameClock = 0;
	gameDeltaTime = 0;
}

