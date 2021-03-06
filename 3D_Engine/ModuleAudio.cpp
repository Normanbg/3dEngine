#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "Brofiler/Brofiler.h"


#include <algorithm>
#include "mmgr/mmgr.h"


#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled), music(NULL)
{
	name = "Audio";
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init(JSON_Object* obj)
{
	BROFILER_CATEGORY("Audio_Init", Profiler::Color::Beige);
	OWN_LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);


	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		OWN_LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// load support for the OGG format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		OWN_LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		OWN_LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = false;
	}

	json_object_clear(obj);//clear obj to free memory
	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	OWN_LOG("Freeing sound FX, closing Mixer and Audio subsystem");
	BROFILER_CATEGORY("Audio_CleanUp", Profiler::Color::Beige);
	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	std::vector<Mix_Chunk*>::iterator item;

	for(item = fx.begin(); item != fx.end(); item++)
	{
		Mix_FreeChunk(*item);
	}

	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;
	
	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic((int) (fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		OWN_LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				OWN_LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				OWN_LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	OWN_LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;
	std::string rootPath = ENGINE_FX_PATH;
	rootPath += path; 

	Mix_Chunk* chunk = Mix_LoadWAV(rootPath.c_str());

	if(chunk == NULL)
	{
		OWN_LOG("Cannot load wav %s. Mix_GetError(): %s", rootPath.c_str(), Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	Mix_Chunk* chunk = fx[id-1];
	if (chunk != NULL)
	{
		Mix_PlayChannel(-1, chunk, repeat);
		ret = true;
	}
	else
		OWN_LOG("Fx not founded on the fx list");

	return ret;
}