#include "FakeFunctions.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

FakeFunctions::FakeFunctions()
{
}

FakeFunctions::~FakeFunctions()
{
}

void FakeFunctions::ExecuteFunction(Functions function)
{
	switch (function)
	{
	case NO_FUNCTION:
		break;
	case START:
		DoStart();
		break;	
	case CLOSE_IN_GAME_MENU:
		DoCloseGameMenu();
		break;
	case ACTIVATE_VSYNC:
		DoActivateVsync();
		break;
	default:
		break;
	}
}

void FakeFunctions::DoStart()
{
}

void FakeFunctions::DoOpenGameMenu()
{
}

void FakeFunctions::DoCloseGameMenu()
{
}

void FakeFunctions::DoActivateVsync()
{
	App->renderer3D->SetVsync();
}

const char * FakeFunctions::FunctionToString(int i)
{
	switch (i)
	{
	case 0:
		return "No Function";
	case 1:
		return "Start";	
	case 2:
		return "Close Game Menu";
	case 3:
		return "Activate VSync";
	}
	return nullptr;
}
