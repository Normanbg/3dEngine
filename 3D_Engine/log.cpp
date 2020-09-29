#pragma once
#include "ModuleGUI.h"
#include "Application.h"
#include "Globals.h"


#include "mmgr/mmgr.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;
	
	const char* shortNameFile = strrchr(file, '\\') + 1;//searches for the last char '\\' in the file dir and stores the str from that point without this char into a pointer (for this reason is +1)
	
	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "%s(%d) : %s", shortNameFile, line, tmp_string);
	OutputDebugString(tmp_string2);

	if (App)
		App->gui->AddConsoleLogs(tmp_string2);
}
