#include "Application.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "DeviceId\DeviceId.h"

#include "./JSON/parson.h"

Application::Application()
{
	frames = 0;

	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
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
	JSON_Object* obj;
	JSON_Object* appObj;
	if (config = json_parse_file("config.JSON")) {
		OWN_LOG("Config.JSON File detected");
		obj = json_value_get_object(config);
		appObj = json_object_get_object(obj, "App");

		const char* title = json_object_get_string(appObj, "Name");
		window->SetTitle((char*)title);

		const char* title2 = json_object_get_string(appObj, "Organization");
		SetOrganization((char*)title2);

		if (json_object_has_value(obj, "App")){
			int i = 30;

		}
	}

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
	
	/*JSON_Value* value;
	JSON_Object* obj;

	if (value = json_parse_file("package.JSON")) {
	
		
		obj = json_value_get_object(value);
		const char* str = json_object_get_string(obj, "name");
		const char* str2 = json_object_get_string(obj, "description");
		json_object_set_string(obj, "name", "newName");
		const char* str3 = json_object_get_string(obj, "name");
		
		json_serialize_to_file(value, "NewJSON.JSON");

		int i = 10;

		json_value_free(value);
		
	}*/
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

	
}

// ---------------------------------------------
void Application::FinishUpdate()
{
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
		totalVideoMemF		=	(float)totVideoMem / (1024.0f * 1024.0f);
		currentVideoMemF	=	(float)currVideoMem / (1024.0f * 1024.0f);
		availableVideoMemF	=	(float)availbVideoMem / (1024.0f * 1024.0f);
		reservedVideoMemF	=	(float)reserVideoMem / (1024.0f * 1024.0f);
	}
}

void Application::HardwareData(){
if (ImGui::CollapsingHeader("Hardware"))
{
	GetHardWareData();

	SDL_version version;
	SDL_GetVersion(&version);

	ImVec4 yellow(1, 1, 0, 1);

	ImGui::Text("SDL Version"); ImGui::SameLine();
	ImGui::TextColored(yellow, "%d.%d.%d", version.major, version.minor, version.patch);
	ImGui::Separator();
	ImGui::Text("CPU's:"); ImGui::SameLine();
	ImGui::TextColored(yellow, "%d", SDL_GetCPUCount()); ImGui::SameLine();
	ImGui::TextColored(yellow, "(Cache: %dkb)", SDL_GetCPUCacheLineSize());

	ImGui::Text("System RAM:"); ImGui::SameLine(); 
	ImGui::TextColored(yellow, "%0.1fGb", ((float)SDL_GetSystemRAM() / 1024));

	ImGui::Text("Caps: "); ImGui::SameLine();

	if (SDL_Has3DNow()) ImGui::TextColored(yellow, "3DNow,"); ImGui::SameLine();
	if (SDL_HasAVX()) ImGui::TextColored(yellow, "AVX,"); ImGui::SameLine();
	if (SDL_HasAltiVec()) ImGui::TextColored(yellow, "AltiVec,"); ImGui::SameLine();
	if (SDL_HasMMX()) ImGui::TextColored(yellow, "MMX,"); ImGui::SameLine();
	if (SDL_HasRDTSC()) ImGui::TextColored(yellow, "RDTSC,"); ImGui::SameLine();
	if (SDL_HasSSE()) ImGui::TextColored(yellow, "SSE,"); ImGui::SameLine();
	if (SDL_HasSSE2()) ImGui::TextColored(yellow, "SSE2,"); ImGui::SameLine();
	if (SDL_HasSSE3()) ImGui::TextColored(yellow, "SSE3,"); ImGui::SameLine();
	if (SDL_HasSSE41()) ImGui::TextColored(yellow, "SSE41,"); ImGui::SameLine();
	if (SDL_HasSSE42()) ImGui::TextColored(yellow, "SSE42,");

	ImGui::Separator();

	ImGui::Text("GPU: "); ImGui::SameLine();
	ImGui::TextColored(yellow, " vendor %s", renderer3D->GetGraphicsVendor());

	ImGui::Text("Brand: "); ImGui::SameLine();
	ImGui::TextColored(yellow, "%s", renderer3D->GetGraphicsModel());

	ImGui::Text("VRAM Budget: "); ImGui::SameLine();
	ImGui::TextColored(yellow, "%.2f Mb", totalVideoMemF);

	ImGui::Text("VRAM Usage: "); ImGui::SameLine();
	ImGui::TextColored(yellow, "%.2f Mb", currentVideoMemF);

	ImGui::Text("VRAM Avaliable: "); ImGui::SameLine();
	ImGui::TextColored(yellow, "%.2f Mb", availableVideoMemF);

	ImGui::Text("VRAM Reserved: "); ImGui::SameLine();
	ImGui::TextColored(yellow, "%.2f Mb", reservedVideoMemF);

	/*{ FOR MAKING A GRAPHIC, DON'T ERASE!!!!!!!!!!!
		char buf[32];
		sprintf(buf, "%.2f/%.2f", currentVideoMemF, availableVideoMemF);
		float percentage = (currentVideoMemF * 100) / availableVideoMemF;

		ImGui::ProgressBar(percentage / 100, ImVec2(0.f, 0.f), buf);
		ImGui::SameLine();
		ImGui::Text("VRAM usage");
	}*/
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