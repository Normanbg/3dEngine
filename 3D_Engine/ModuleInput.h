#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__

#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum FileType {
	CANT_LOAD = 0,

	FBX,
	PNG,
	JPG,
	DDS,
	PEI,
	TGA
};

class ModuleInput : public Module
{
public:

	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init(JSON_Object* obj);
	update_status PreUpdate(float dt);
	bool CleanUp();

	FileType ObtainDroppedFileType(std::string droppedFileDir);

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;

	char* dropped_filedir = nullptr;
	uint dropFX = 0;

};

#endif // !__MODULEINPUT_H__
