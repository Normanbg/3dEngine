#pragma once

#include <list>
#include <vector>
#include"./ImGui/imgui.h"
#include "Globals.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleGui.h"

#define MAX_FPS_LOG 50
#define MAX_MS_LOG 50
#define FRAMERATE_CAP 60

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleGui* gui;

private:

	
	float	dt = .0f;
	uint	frames = 0;
	float	time_scale = 1.0f;
	float	prevTime_scale = 1.f;
	int		time_scaleFrames = -1;

	PerfTimer	ptimer;
	Uint64		frame_count = 0;
	Uint32		framerate_cap = FRAMERATE_CAP;
	Uint32		prev_framerate_cap = 0;
	Uint32		frames_to_framerate_reset = -1;
	Uint32		delta_time = 0;
	Timer		delay_time;
	Timer		startup_time;
	Timer		ten_sec_timer;
	Timer		ms_timer;
	Timer		last_sec_frame_time;
	Uint32		last_sec_frame_count = 0;
	Uint32		prev_last_sec_frame_count = 0;

	void StoreFpsLog();
	void StoreMsLog();


	std::list<Module*> list_modules;


	float totalVideoMemF = -1.0f;
	float currentVideoMemF = -1.0f;
	float availableVideoMemF = -1.0f;
	float reservedVideoMemF = -1.0f;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(std::string link);

	
	std::vector<float> GetFpsLog();
	std::vector<float> GetMsLog();
	
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	Uint32 GetFramerateCap() const;
	void SetFramerateCap(Uint32 cap, int frames = -1);
	float avg_fps;
	float seconds_since_startup;
	Uint32 last_frame_ms;
	Uint32 frames_on_last_update;

	float GetTimeScale()const;
	void SetTimeScale(float ts, int frameNumber = -1);
	void PauseGame(bool pause);

	void GetHardWareData();
	void HardwareData();

	void SetOrganization(char* newName);
	std::string GetOrganization() const;

private:

	std::string _organization;

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();


};
extern Application* App;