#include "PerfTimer.h"
#include "./SDL/include/SDL_timer.h"

#include "mmgr/mmgr.h"

double PerfTimer::frequency = 0;

PerfTimer::PerfTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

void PerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();
}

double PerfTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

double PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}


