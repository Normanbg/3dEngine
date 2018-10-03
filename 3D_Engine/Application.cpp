#include "Application.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "DeviceId\DeviceId.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleGui.h"

#include "./JSON/parson.h"


Application::Application()
{
	frames = 0;
	want_to_save = want_to_load = false;

	window = new ModuleWindow();
	input = new ModuleInput(this);
	audio = new ModuleAudio(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	gui = new ModuleGui(this);
	
	_organization = ORGANIZATION;
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(gui);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list < Module* > ::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++) {
		delete (*item);
	}
	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;
	JSON_Value* config;
	JSON_Object* objModules = nullptr;
	
	if (config = json_parse_file(CONFIG_FILE)) {
		OWN_LOG("Config.JSON File detected");
		JSON_Object* obj;
		JSON_Object* appObj;
		

		obj = json_value_get_object(config);
		appObj = json_object_get_object(obj, "App");
		
		SetDataFromJson(appObj);

		objModules = obj;
		json_object_clear(appObj);
		obj = nullptr;
		json_object_clear(obj);
		
	}

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();


	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init(json_object_get_object(objModules, (*item)->name.c_str()));
		item++;
	}

	// After all Init calls we call Start() in all modules
	OWN_LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}

	
	json_object_clear(objModules);
	json_value_free(config);

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	delta_time = ms_timer.Read();
	dt = (delta_time / 1000.0f) * time_scale;
	if (time_scaleFrames > 0) {
		time_scaleFrames--;
	}
	else if (time_scaleFrames == 0) {
		time_scale = prevTime_scale = 1.f;
		time_scaleFrames = -1;
	}
	ms_timer.Start();

	GetHardWareData();
	
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (want_to_save == true) SavegameNow();

	if (want_to_load == true) LoadGameNow();

	if (last_sec_frame_time.Read() > 1000) {
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	avg_fps = float(frame_count) / ten_sec_timer.ReadSec();
	ten_sec_timer.ReadSec() >= 10 ? ten_sec_timer.Start(), frame_count = 0 : ' '; //This is questionable
	seconds_since_startup = startup_time.ReadSec();
	last_frame_ms = ms_timer.Read();
	frames_on_last_update = prev_last_sec_frame_count;


	/*static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frame_count);
	window->SetTitle(title);*/

	delay_time.Start();
	int delay_ms = (1000 / framerate_cap) - last_frame_ms;
	if (delay_ms > 0)
		SDL_Delay(delay_ms);
	if (frames_to_framerate_reset-- == 0)
		framerate_cap = prev_framerate_cap;
	uint wait_ms = delay_time.Read();


	StoreFpsLog();
	StoreMsLog();
}

void Application::GetHardWareData()
{
	uint64_t totVideoMem, currVideoMem, availbVideoMem, reserVideoMem;

	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &totVideoMem, &currVideoMem, &availbVideoMem, &reserVideoMem))	{
		//OWN_LOG("Succesfully get graphics device data")
		
		totalVideoMemF =	(float)totVideoMem / (1024.0f * 1024.0f);
		currentVideoMemF =	(float)currVideoMem / (1024.0f * 1024.0f);
		availableVideoMemF = (float)availbVideoMem / (1024.0f * 1024.0f);
		reservedVideoMemF = (float)reserVideoMem / (1024.0f * 1024.0f);
	}
}


// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->PreUpdate(dt);
	}
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->Update(dt);
	}
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		ret = (*item)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::RequestBrowser(std::string link)
{
	ShellExecute(NULL, "open", link.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::StoreFpsLog() {
	
	if (fps_log.size() < MAX_FPS_LOG) {
		fps_log.push_back(frames_on_last_update);
	}
	else {
		fps_log.erase(fps_log.begin());
		fps_log.push_back(frames_on_last_update);
	}
}
void Application::StoreMsLog() {

	if (ms_log.size() < MAX_MS_LOG) {
		ms_log.push_back(last_frame_ms);
	}
	else {
		ms_log.erase(ms_log.begin());
		ms_log.push_back(last_frame_ms);
	}
}

std::vector<float> Application::GetFpsLog() {
	return fps_log;
}
std::vector<float> Application::GetMsLog() {
	return ms_log;
}

Uint32 Application::GetFramerateCap() const
{
	return framerate_cap;
}

void Application::SetFramerateCap(Uint32 cap, int frames)
{
	prev_framerate_cap = framerate_cap;
	framerate_cap = cap;
	frames_to_framerate_reset = frames;
}

float Application::GetTimeScale() const
{
	return time_scale;
}

void Application::SetTimeScale(float ts, int frameNumber)
{
	prevTime_scale = time_scale;
	time_scale = ts;
	time_scaleFrames = frameNumber;
}

void Application::PauseGame(bool pause) {
	SetTimeScale(pause ? 0.f : 1.f);	
}

void Application::SetOrganization(char* newName)
{
	_organization = newName;
}



 std::string Application::GetOrganization() const
{
	return _organization;
}

 void Application::LoadGame()
 {
	 want_to_load = true;
 }

 void Application::SaveGame() const
 {
	 want_to_save = true;
 }

 bool Application::LoadGameNow()
 {
	 
	 bool ret = false;

	 JSON_Value* config;	 

	 if (config = json_parse_file(CONFIG_FILE)) {
		 OWN_LOG("Config.JSON File detected");
		 JSON_Object* obj;
		 JSON_Object* appObj;


		 obj = json_value_get_object(config);
		 appObj = json_object_get_object(obj, "App");

		 SetDataFromJson(appObj);

		 
		 json_object_clear(appObj);
		
		std::list<Module*>::iterator item = list_modules.begin();

		 while (item != list_modules.end())
		 {
			 ret &= (*item)->Load(json_object_get_object(obj, (*item)->name.c_str()));
			 item++;
		 }
		
		 json_object_clear(obj);
	 }
	 	 
	 json_value_free(config);
	 
	 want_to_load = false;
	 return ret;
 }

 bool Application::SavegameNow() const
 {
	 bool ret = false;

	 JSON_Value* value= json_value_init_object();
	 JSON_Object* obj = json_value_get_object(value);
	 
	 json_object_dotset_string(obj, "App.Name", window->GetWindowTitle().c_str());
	 json_object_dotset_string(obj, "App.Organization", GetOrganization().c_str());
	 

	 std::list<Module*>::const_iterator item = list_modules.begin();

	 while (item != list_modules.end())
	 {
		 ret &= (*item)->Save(obj);
		 item++;
	 }

	 json_serialize_to_file(value, CONFIG_FILE);
	 json_object_clear(obj);
	 json_value_free(value);

	 want_to_save = false;
	 return ret;
 }

 void Application::SetDataFromJson(JSON_Object* data) {
 
	 const char* title = json_object_get_string(data, "Name");
	 window->SetTitle((char*)title);

	 const char* title2 = json_object_get_string(data, "Organization");
	 SetOrganization((char*)title2);
 
 }