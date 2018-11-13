#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Config.h"
#include "Brofiler/Brofiler.h"

#include "mmgr/mmgr.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	
	window = NULL;
	screen_surface = NULL;
	name = "Window";

	_fullscreen = WIN_FULLSCREEN;
	_borderless = WIN_BORDERLESS;
	_resizable = WIN_RESIZABLE;
	_fullDesktop = WIN_FULLSCREEN_DESKTOP;

	
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
		

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		OWN_LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{	
		//Create window
		SDL_DisplayMode size;
		SDL_GetDesktopDisplayMode(0, &size);

		int marginX = size.w / 7;
		int marginY = size.h / 5;
		_w = size.w - marginX;
		_h = size.h - marginY;
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

		window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _w, _h, flags);

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

void ModuleWindow::SetTitle(const char* title)
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
	const uint margin = 30;

	SDL_DisplayMode dMode;
	SDL_GetDesktopDisplayMode(0, &dMode);
	if (fulldesktop) {
		
		_w = dMode.w;
		_h = dMode.h;		
	}
	else {	
		int marginX = dMode.w / 7;
		int marginY = dMode.h / 5;
		_w = dMode.w - marginX;
		_h = dMode.h - marginY;
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

bool ModuleWindow::LoadSettings(Config* data) {

	SetBrightness(data->GetFloat("Brightness", 1.0f));
	SetFullscreen(data->GetBool("Fullscreen", false));
	SetResizable(data->GetBool("Resizable", true));
	SetBorderless(data->GetBool("Borderless", false));
	SetFullscreenDesktop(data->GetBool("Fullscreen Desktop", false));
	SetSize(data->GetUInt("Width",SCREEN_WIDTH), data->GetUInt("Height", SCREEN_HEIGHT));

	return true;
}
bool ModuleWindow::SaveSettings(Config* data)const {
	data->AddFloat("Brightness", GetBrightness());
	data->AddBool("Fullscreen", IsFullscreen());
	data->AddBool("Resizable", IsResizable());
	data->AddBool("Borderless", IsBorderless());
	data->AddBool("Fullscreen Desktop", IsFullscreenDesktop());
	int w, h;
	GetSize(w, h);
	data->AddUInt("Width", w);
	data->AddUInt("Height", h);

	return true;
}
