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

	bool Init(JSON_Object* obj);
	bool CleanUp();
	bool Load(JSON_Object* data)override ;
	bool Save(JSON_Object* data) const;

	void SetTitle(char* title);

	void SetBorderless(bool borderless);
	void SetResizable(bool resizable);
	void SetFullscreen(bool fullscreen);
	void SetFullscreenDesktop(bool fullsdesktop);
	void SetSize(uint w, uint h);

	bool IsFullscreen() const { return _fullscreen; }
	bool IsBorderless() const { return _borderless; }
	bool IsResizable() const { return _resizable; }
	bool IsFullscreenDesktop() const{ return _fullDesktop; }
	float GetBrightness() const { return _brightness; }
	std::string GetWindowTitle() const { return _title; }
	void GetSize(int &h, int &w)const;

	void SetBrightness(float bright);

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
	int _brightness = DEFAULT_BRIGHTNESS;
	std::string _title  = TITLE;

	uint _w;
	uint _h;
};

#endif // __ModuleWindow_H__