#pragma once
#ifndef __ModuleGui_H__
#define __ModuleGui_H__
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Module.h"


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


	bool my_tool_active = true;
	float my_color = 0.000;
};

#endif 