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

	bool IsFullscreen() { return fullscreen; }
	bool IsBorderless() { return borderless; }
	bool IsResizable() { return resizable; }
	bool IsFullscreenDesktop() { return fullDesktop; }

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fullDesktop;
};

#endif // __ModuleWindow_H__