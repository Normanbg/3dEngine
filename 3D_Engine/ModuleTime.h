#ifndef __MODULETIME_H__
#define __MODULETIME_H__


#include "Application.h"
#include "Module.h"
#include "Timer.h"

class ModuleTime : public Module
{
public:
	ModuleTime(bool start_enabled = true);
	~ModuleTime();


	bool Init(JSON_Object* obj) override { return true; }
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override { return UPDATE_CONTINUE; }
	update_status PostUpdate(float dt) override { return UPDATE_CONTINUE; }
	bool CleanUp() override { return true; }

	//bool Start();
	void Reset();
	void Play();
	void Resume();
	void Pause();
	void Stop();

	const float GetGameTimeSec()const { return gameClock; }
	const float GetGameDeltaTime() const { return gameDeltaTime; }

	const float GetRealTimeSec() { return realTimeClock.ReadSec(); }
	const float GetRealTimeMs() { return realTimeClock.Read(); }
	const bool IsPaused() const { return paused; }
	const bool IsPlaying() const { return playing; }

	float* GetTimeScale() { return &timeScale; }

private:
	float timeScale = 1.0f;
	float gameDeltaTime = 0.0f;

	Timer realTimeClock;
	float gameClock = 0.0f;
	bool paused = false;
	bool playing = false;

};
#endif // !__MODULETIME_H__
