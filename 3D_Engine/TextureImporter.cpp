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

void TextureImporter::LoadTexture(char * path) {

	ILuint imageID;
	GLuint textureID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ILinfo infoImage;
	iluGetImageInfo(&infoImage);
	
}

void TextureImporter::DrawTexture(){

	

}

void TextureImporter::LoadCheckeredPlane(){
	//----checkerer
	const int h = 20;
	const int w = 20;


	GLubyte checkImage[h][w][4]; // Checkered texture
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
	//--------
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &imageID);//generates a texture buffer 
	glBindTexture(GL_TEXTURE_2D, imageID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage); //specifies the texture
}

void TextureImporter::DrawCheckeredPlane(){

	glColor4f(1.f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, imageID);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.0, 0.0); // creates first triangle of a squared plane with its UVs
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);


	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.0, 0); // second tri
	glTexCoord2f(1.0, 1.0); glVertex3f(1, 1.0, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1.0, 0.0);


	glEnd();

	glDisable(GL_TEXTURE_2D);
}

