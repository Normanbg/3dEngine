#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;
struct Config;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(JSON_Object* obj);
	bool CleanUp();
	bool LoadSettings(Config* data)override ;
	bool SaveSettings(Config* data) const override;

	void SetTitle(const char* title);

	void SetBorderless(bool borderless);
	void SetResizable(bool resizable);
	void SetFullscreen(bool fullscreen);
	void SetFullscreenDesktop(bool fullsdesktop);
	void SetSize(uint w, uint h);

	const bool IsFullscreen() const { return _fullscreen; }
	const bool IsBorderless() const { return _borderless; }
	const bool IsResizable() const { return _resizable; }
	const bool IsFullscreenDesktop() const{ return _fullDesktop; }
	const float GetBrightness() const { return _brightness; }
	const std::string GetWindowTitle() const { return _title; }
	void GetSize(int &h, int &w)const;
	const float2 GetSize();

	void SetBrightness(float bright);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	void SetDataFromJson(JSON_Object* data);

	bool _fullscreen;
	bool _resizable;
	bool _borderless;
	bool _fullDesktop;
	float _brightness = DEFAULT_BRIGHTNESS;
	std::string _title  = TITLE;

	uint _w;
	uint _h;
};

#endif // __ModuleWindow_H__