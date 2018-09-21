#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void SetBorderless(bool borderless);
	void SetResizable(bool resizable);
	void SetFullscreen(bool fullscreen);
	void SetFullscreenDesktop(bool fullsdesktop);

	bool IsFullscreen() { return _fullscreen; }
	bool IsBorderless() { return _borderless; }
	bool IsResizable() { return _resizable; }
	bool IsFullscreenDesktop() { return _fullDesktop; }

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	bool _fullscreen;
	bool _resizable;
	bool _borderless;
	bool _fullDesktop;
};

#endif // __ModuleWindow_H__