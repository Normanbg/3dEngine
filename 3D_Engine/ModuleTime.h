#ifndef __MODULE_TIME_H__
#define __MODULE_TIME_H__
#include "Module.h"
#include "Timer.h"

class ModuleTime : public Module
{
public:
	ModuleTime();
	~ModuleTime();

	bool Start();
	void Reset();
	void Play();
	void Pause();


	Timer realTimeClock;
	Timer gameClock;
	
};

#endif // __MODULE_TIME_H__