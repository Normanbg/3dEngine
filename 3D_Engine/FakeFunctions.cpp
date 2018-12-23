#include "FakeFunctions.h"

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
	case OPEN_IN_GAME_MENU:
		DoOpenGameMenu();
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
}
