#pragma once
#ifndef __ModuleGui_H__
#define __ModuleGui_H__

#include "Globals.h"
#include "Module.h"
#include <list>


#include "RandomGenerator/pcg_variants.h"

class UIPanel;
class UIPanelAbout;
class UIPanelConfig;
class UIPanelConsole;

class ModuleGui : public Module
{
public:
	
	ModuleGui(bool start_enabled = true);
	~ModuleGui();

	bool  Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void AddConsoleLogs(const char* log);

public:
	bool demoShowcase = false;

	std::list<UIPanel*> uiPanels;
	UIPanelAbout* panelAbout = nullptr;
	UIPanelConfig* panelConfig = nullptr;
	UIPanelConsole* panelConsole = nullptr;

	std::list<const char*>logsBuffer;

};

#endif 