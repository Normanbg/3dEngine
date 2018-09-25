#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <list>
#include <vector>
#include"./ImGui/imgui.h"
#include "SDL\include\SDL.h"
#include "Globals.h"
#include "PerfTimer.h"
#include "Timer.h"


#define MAX_FPS_LOG 50
#define MAX_MS_LOG 50
#define FRAMERATE_CAP 60

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModulePhysics3D;
class ModuleGui;

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

	bool LoadGameNow();
	bool SavegameNow() const;

	mutable bool		want_to_save;
	bool				want_to_load;
	std::string			load_game;
	mutable std::string	save_game;

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

	void LoadGame();
	void SaveGame() const;

	
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

	void GetDataFromJson(JSON_Object* data);


};

extern Application* App;

#endif