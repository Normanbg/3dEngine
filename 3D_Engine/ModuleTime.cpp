#include "ModuleTime.h"

ModuleTime::ModuleTime()
{

}


ModuleTime::~ModuleTime()
{
}

bool ModuleTime::Start()
{
	realTimeClock.Start();
	gameClock.Start();

	return true;
}

void ModuleTime::Reset()
{
	realTimeClock.SetZero();
	gameClock.SetZero();
}

void ModuleTime::Play()
{
	if (gameClock.isPaused()) {
		gameClock.Start();
	}
}

void ModuleTime::Pause()
{
	gameClock.Stop();

}


