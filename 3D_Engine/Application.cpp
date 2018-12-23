#include "Application.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "DeviceId/DeviceId.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "ModuleTime.h"
#include "ModuleEditorCamera.h"
#include "ModuleResources.h"
#include "ModuleGui.h"
#include "SceneImporter.h"
#include "TextureImporter.h"
#include "FontManager.h"
#include "Brofiler/Brofiler.h"

#include "mmgr/mmgr.h"

#include "./JSON/parson.h"


Application::Application()
{
	frames = 0;
	want_to_save = want_to_load = false;

	window = new ModuleWindow();
	input = new ModuleInput(this);
	audio = new ModuleAudio(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleEditorCamera(this);
	gui = new ModuleGui(this);
	scene = new ModuleScene(this);
	fileSys = new ModuleFileSystem(this);
	time = new ModuleTime();
	resources = new ModuleResources();
	
	_organization = ORGANIZATION;
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(input);
	AddModule(window);
	AddModule(camera);
	AddModule(fileSys);
	AddModule(audio);
	AddModule(resources);
	AddModule(scene);
	AddModule(time);
	
	

	AddModule(gui);
	
	// Scenes

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application(){
	for (std::list < Module* > ::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++) {
		delete (*item);
	}
	list_modules.clear();
	RELEASE(fontManager);
	RELEASE(importer);
	RELEASE(texImporter);
}

bool Application::Init(){

	BROFILER_CATEGORY("App_Init", Profiler::Color::DarkOrange);
	bool ret = true;
	LoadGameNow();

	importer = new SceneImporter();
	texImporter = new TextureImporter();
	fontManager = new FontManager();

	importer->Init();
	texImporter->Init();

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init(nullptr);
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
	fontManager->LoadAllFolderFonts();
	App->scene->LoadScene("Scenes/Intro.JSON");


	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	BROFILER_CATEGORY("App_PrepareUpdate", Profiler::Color::DarkRed);
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
	BROFILER_CATEGORY("App_FinishUpdate", Profiler::Color::DarkCyan);
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

	ret = doPreUpdate();
	if (ret != UPDATE_CONTINUE) {
		return ret;
	}
	ret = doUpdate();
	if (ret != UPDATE_CONTINUE) {
		return ret;
	}
	ret = doPostUpdate();
	if (ret != UPDATE_CONTINUE) {
		return ret;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	BROFILER_CATEGORY("App_CleanUp", Profiler::Color::DarkGray);

	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}

	fontManager->CleanUp();

	importer->CleanUp();

	return ret;
}

void Application::RequestBrowser(const char* link) const{
	ShellExecute(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
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

std::vector<float> Application::GetFpsLog() const  {
	return fps_log;
}
std::vector<float> Application::GetMsLog()const {
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

const float Application::GetTimeScale() const
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

void Application::SetOrganization(const char* newName)
{
	_organization = newName;
}



 const std::string Application::GetOrganization() const
{
	return _organization;
}

 void Application::LoadGame()
 {
	 want_to_load = true;
 }

 void Application::SaveGame()
 {
	 want_to_save = true;
 }

 bool Application::LoadGameNow()
 {
	 bool ret = false;
	 
	 char* buffer = nullptr;
	 uint size = App->fileSys->readFile(CONFIG_FILE, &buffer);
	 

	 if (size < 0) {
		 OWN_LOG("Error loading file %s. All data not loaded.", CONFIG_FILE)
			 return ret;
	 }
	 Config conf(buffer);
	 RELEASE_ARRAY(buffer);

	 Config appdata = conf.GetArray("App", 0);
	 std::string name = appdata.GetString("Name", "NoName");
	 window->SetTitle(name.c_str());
	 std::string org = appdata.GetString("Organization", "NoOrganization");
	 SetOrganization(org.c_str());
	 

	 std::list<Module*>::iterator item = list_modules.begin();

	 while (item != list_modules.end())
	 {		 
		 Config elem = conf.GetArray((*item)->name.c_str(), 0);
		 ret &= (*item)->LoadSettings(&elem);
		 item++;
		
	 }
	 want_to_load = false;
	 return ret;
 }

 bool Application::SavegameNow() const
 {
	 bool ret = false;

	 Config save;

	 save.AddArray("App");
	 Config appdata;
	 appdata.AddString("Name", window->GetWindowTitle().c_str());
	 appdata.AddString("Organization", GetOrganization().c_str());
	 save.AddArrayChild(appdata);
	 
	 
	 
	 std::list<Module*>::const_iterator item = list_modules.begin();

	 while (item != list_modules.end())
	 {
		 Config module;
		save.AddArray((*item)->name.c_str());
		
		 ret &= (*item)->SaveSettings(&module);
		 item++;		
		 save.AddArrayChild(module);
	 }
	 //save.AddArrayChild(module);

	 char* buffer = nullptr;
	 uint size = save.Save(&buffer);

	 App->fileSys->writeFile(CONFIG_FILE, buffer, size);
	 OWN_LOG("Saving config.")
	
	
	 want_to_save = false;
	 return ret;
 }

 update_status Application::doPreUpdate() {
	 BROFILER_CATEGORY("App_PreUpdate", Profiler::Color::DarkKhaki);
	 update_status ret = UPDATE_CONTINUE;
	 for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {

		 ret = (*item)->PreUpdate(dt);
	 }

	 return ret;
 }

 update_status Application::doUpdate() {
	 BROFILER_CATEGORY("App_Update", Profiler::Color::DarkGoldenRod);
	 update_status ret = UPDATE_CONTINUE;
	 for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		  ret = (*item)->Update(dt);
	 }
	 return ret;
 }

 update_status Application::doPostUpdate(){
	 BROFILER_CATEGORY("App_PostUpdate", Profiler::Color::DarkSalmon);
	 update_status ret = UPDATE_CONTINUE;
	 for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++) {
		 ret = (*item)->PostUpdate(dt);
	 }
	 return ret;
 }

 void Application::BroadcastEvent(const Event& event)
 {
	 for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it)
		 (*it)->ReceiveEvent(event);
	 texImporter->ReceiveEvent(event);
 }