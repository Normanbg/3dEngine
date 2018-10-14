#ifndef __ModuleGui_H__
#define __ModuleGui_H__

#include "Globals.h"
#include "Module.h"
#include <list>
#include <vector>


#include "RandomGenerator/pcg_variants.h"

union SDL_Event;

class UIPanel;
class UIPanelAbout;
class UIPanelConfig;
class UIPanelConsole;
class UIPanelProperties;

class ModuleGui : public Module
{
public:
	
	ModuleGui(bool start_enabled = true);
	~ModuleGui();

	bool  Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);	
	bool CleanUp();

	void Draw();


	void AddConsoleLogs(const char* log) ;
	void ImplGuiInputs(SDL_Event* e)const;
	bool MouseOnGui() const;

public:
	bool demoShowcase = false;

	std::list<UIPanel*> uiPanels;
	UIPanelAbout*		panelAbout		= nullptr;
	UIPanelConfig*		panelConfig		= nullptr;
	UIPanelConsole*		panelConsole	= nullptr;
	UIPanelProperties*	panelProperties	= nullptr;

	std::vector<std::string> logsBuffer;
	uint ilVersion = 0;

	uint dropFX = 0;
	uint closeFX = 0;
	uint openFX = 0;
	uint searchingFX = 0; //Used when open an URL

};

#endif 