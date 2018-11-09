#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"


class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	void Resume();

	UINT32 Read();
	float ReadSec();

	bool isPaused();
	void SetZero();

private:

	bool	running;
	float	started_at;
	float	stopped_at;
};

#endif //__TIMER_H__