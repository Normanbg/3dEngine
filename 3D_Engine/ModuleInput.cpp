#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGui.h"
#include "Brofiler/Brofiler.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	name = "Input";
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init(JSON_Object* obj)
{
	BROFILER_CATEGORY("Input_Init", Profiler::Color::GhostWhite);
	OWN_LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		OWN_LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	json_object_clear(obj);//clear obj to free memory
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Input_PreUpdate", Profiler::Color::GhostWhite);
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		App->gui->ImplGuiInputs(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:

				if (!App->gui->MouseOnGui())

					mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			if(e.window.event == SDL_WINDOWEVENT_RESIZED)
			App->renderer3D->OnResize(e.window.data1, e.window.data2);
			break;
			case SDL_DROPFILE:
				dropped_filedir = e.drop.file;
				std::string dropped_filedirStr(e.drop.file);
				App->audio->PlayFx(App->gui->dropFX);
				switch (FileType file = ObtainDroppedFileType(dropped_filedirStr))
				{
				case CANT_LOAD:
					OWN_LOG("File not supported, try FBX, PNG, JPG or DDS")
					break;
				case FBX:
					OWN_LOG("Dropped .fbx file");
					App->renderer3D->LoadDroppedFBX(dropped_filedir);
					
					break;
				case PNG:
					OWN_LOG("Dropped .png file");
					App->renderer3D->importer->ChangeMeshTexture(dropped_filedir);
					
					break;
				case JPG:
					OWN_LOG("Dropped .jpg file");
					App->renderer3D->importer->ChangeMeshTexture(dropped_filedir);

					break;
				case DDS:
					OWN_LOG("Dropped .dds file");
					App->renderer3D->importer->ChangeMeshTexture(dropped_filedir);
					
					break;
				default:
					break;
				}
			break;
		}
	}

	if (quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;		

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModuleInput::CleanUp()
{
	BROFILER_CATEGORY("Input_CleanUp", Profiler::Color::GhostWhite);
	OWN_LOG("Quitting SDL input event subsystem");
	SDL_free(dropped_filedir);
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

FileType ModuleInput::ObtainDroppedFileType(std::string droppedFileDir){
	std::string dFile = droppedFileDir;
	if (dFile.length() > 4) {
		std::string formatStr = dFile.substr(dFile.length() - 3);
		if (formatStr == "fbx" || formatStr == "FBX")
			return FBX;
		else if (formatStr == "png" || formatStr == "PNG")
			return PNG;
		else if (formatStr == "jpg" || formatStr == "JPG")
			return JPG;
		else if (formatStr == "dds" || formatStr == "DDS")
			return DDS;
	}
	else
		OWN_LOG("Cannot load %s file.  Format not recognized", droppedFileDir)
		return CANT_LOAD;
}
