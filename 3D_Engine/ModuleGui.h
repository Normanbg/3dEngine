#pragma once
#ifndef __ModuleGui_H__
#define __ModuleGui_H__

#include "Globals.h"
#include "Primitive.h"
#include "Module.h"

#include "RandomGenerator/pcg_variants.h"

class ModuleGui : public Module
{
public:
	
	ModuleGui(Application* app, bool start_enabled = true);
	~ModuleGui();

	bool  Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	//void ConsoleLogs(std::string log);

public:
	bool demoShowcase = false;
	bool activeAbout = false;
	bool configActive = true;
	bool consoleActive = true;

};

#endif 