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
	ilShutDown();
}

void TextureImporter::Init(){
	App->gui->ilVersion = IL_VERSION; //Used on panel about
	OWN_LOG("Init Image library using DevIL lib version %d", IL_VERSION);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

void TextureImporter::LoadTexture(char * path){

	ILuint imageID;
	GLuint textureID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ILinfo infoImage;
	iluGetImageInfo(&infoImage);

	/*glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);*/
}
