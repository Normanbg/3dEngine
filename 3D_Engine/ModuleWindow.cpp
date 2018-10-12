#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Brofiler/Brofiler.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	
	window = NULL;
	screen_surface = NULL;
	name = "Window";

	_fullscreen = WIN_FULLSCREEN;
	_borderless = WIN_BORDERLESS;
	_resizable = WIN_RESIZABLE;
	_fullDesktop = WIN_FULLSCREEN_DESKTOP;

	_w = SCREEN_WIDTH;
	_h = SCREEN_HEIGHT;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JSON_Object* obj)
{
	OWN_LOG("Init SDL window & surface");
	bool ret = true;

	if (obj != nullptr) {		
		SetDataFromJson(obj);
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		OWN_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
	
		//Create window
		int width = _w * SCREEN_SIZE;
		int height = _h * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		//Use OpenGL 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);


		if(_fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(_resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(_borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(_fullDesktop == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			OWN_LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	json_object_clear(obj);//clear obj to free memory

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	OWN_LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(char* title)
{
	_title = title;
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen(bool fullscr) {

	Uint32 fullsFlag = fullscr ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_MINIMIZED;
	_fullscreen = fullscr;
	SDL_SetWindowFullscreen(window, fullsFlag);
}
void ModuleWindow::SetResizable(bool resizable) {
	_resizable = resizable;
	SDL_bool res = SDL_bool(resizable);
	SDL_SetWindowResizable(window, res);
}
void ModuleWindow::SetBorderless(bool borderless) {
	_borderless = borderless;
	SDL_bool bord = SDL_bool(!borderless);
	SDL_SetWindowBordered(window, bord);
}
void ModuleWindow::SetFullscreenDesktop(bool fulldesktop) {
	_fullDesktop = fulldesktop;
	uint x = 0;
	uint y = 0;
	uint margin = 30;
	
	if (fulldesktop) {
		SDL_DisplayMode dMode;
		SDL_GetDesktopDisplayMode(0, &dMode);
		_w = dMode.w;
		_h = dMode.h;		
	}
	else {		
		_w = SCREEN_WIDTH;
		_h = SCREEN_HEIGHT;
		x = y = margin;		
	}
	SDL_SetWindowSize(window, SCREEN_SIZE*_w, SCREEN_SIZE*_h);
	SDL_SetWindowPosition(window, x, y);

}

void ModuleWindow::SetBrightness(float bright) {

	_brightness = bright;
	SDL_SetWindowBrightness(window, bright);
}

void ModuleWindow::SetSize(uint w, uint h) {
	_h = h;
	_w = w;
	SDL_SetWindowSize(window, w, h);
	
}

void ModuleWindow::GetSize(int &w, int &h)const {

	w = _w;
	h = _h;
}

bool ModuleWindow::Load(JSON_Object* data) {

	SetDataFromJson(data);
	
	SetBrightness(_brightness);
	
	SetFullscreen(_fullscreen);
	SetResizable(_resizable);
	SetBorderless(_borderless);
	SetFullscreenDesktop(_fullDesktop);
	SetSize(_w, _h);

	return true;
}
bool ModuleWindow::Save(JSON_Object* data)const {
	json_object_dotset_number(data, "Window.Brightness", _brightness );
	json_object_dotset_number(data, "Window.Width", _w);
	json_object_dotset_number(data, "Window.Height", _h);
	json_object_dotset_boolean(data, "Window.Fullscreen", _fullscreen);
	json_object_dotset_boolean(data, "Window.Borderless", _borderless);
	json_object_dotset_boolean(data, "Window.Resizable", _resizable);
	json_object_dotset_boolean(data, "Window.Fullscreen Desktop", _fullDesktop);
	return true;
}

void ModuleWindow::SetDataFromJson(JSON_Object* data) {

	_brightness = json_object_dotget_number(data, "Brightness");
	_w = json_object_dotget_number(data, "Width");
	_h = json_object_dotget_number(data, "Height");
	_fullscreen = json_object_dotget_boolean(data, "Fullscreen");
	_borderless = json_object_dotget_boolean(data, "Borderless");
	_resizable = json_object_dotget_boolean(data, "Resizable");
	_fullDesktop = json_object_dotget_boolean(data, "Fullscreen Desktop");

}
