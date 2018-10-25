#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Application.h"
#include "Module.h"


class ModuleTextures :	public Module {
public:
	ModuleTextures(bool start_enabled = true);
	~ModuleTextures();

	bool Init(JSON_Object* obj) override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;


};

#endif __MODULETEXTURES_H__