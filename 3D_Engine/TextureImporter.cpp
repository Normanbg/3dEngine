#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "TextureImporter.h"
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"


#pragma comment( lib, "Devil/libx86/DevIL.lib" )
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" )



TextureImporter::TextureImporter()
{
	
}


TextureImporter::~TextureImporter()
{
}

void TextureImporter::Init(){
	App->gui->ilVersion = IL_VERSION;
	OWN_LOG("Init Image library using DevIL lib version %d", IL_VERSION);
	
}

void TextureImporter::LoadTexture(char * path){
	
}
