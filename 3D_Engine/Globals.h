#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>

#define OWN_LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI


typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define SCREEN_SIZE 1
#define DEFAULT_BRIGHTNESS 1.0f
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "3D_Engine"
#define ORGANIZATION "UPC CITM"
#define CONFIG_FILE "config.JSON"
#define CAMERA_SPEED 10.0f

#define NUM_PATHS 6
#define TEXTURES_PATH "Assets/Textures/"
#define MODELS_PATH "Assets/3DModels/"
#define AUDIO_PATH "Assets/Audio/"
#define LIB_TEXTURES_PATH "Library/Textures/"
#define LIB_MODELS_PATH "Library/3DModels/"
#define SETTINGS_PATH "Settings/"
#define OWN_FILE_FORMAT ".pei"
#define DDS_FORMAT ".dds"