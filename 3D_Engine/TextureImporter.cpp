#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "TextureImporter.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#include "DevIL\include\ilu.h"


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


GLuint TextureImporter::LoadTexture(const char * path, uint& texWidth, uint& texHeight)
{
	OWN_LOG("Loading Texture from %s", path);
	ILuint imageID;

	ilGenImages(1, &imageID); // generates an image
	ilBindImage(imageID);

	bool ret = ilLoadImage(path);
	if (ret) {

		ILinfo infoImage;
		iluGetImageInfo(&infoImage);

		if (infoImage.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		ret = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!ret) {
			OWN_LOG("Cannot convert image. Error: %s", iluErrorString(ilGetError()))
				return -1;
		}

		GLuint textureID;
		texHeight = infoImage.Height;
		texWidth = infoImage.Width;
		
		glGenTextures(1, &textureID);//generates a texture buffer 
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		GLfloat maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);//anisotropic Filter

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, infoImage.Width, infoImage.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData()); //specifies the texture

		ilDeleteImages(1, &imageID);

		glBindTexture(GL_TEXTURE_2D, textureID);
		
		return textureID;
	}
	else {

		OWN_LOG("Cannot load texture from path. Error: %s", iluErrorString(ilGetError()))
			return -1;

	}
}



void TextureImporter::ImportToDDS( const char* path) {

	OWN_LOG("Loading Texture from %s", path);
	ILuint imageID;

	std::string nwPath = TEXTURES_PATH;
	nwPath += path;
	nwPath += ".png";
	ilGenImages(1, &imageID); // generates an image
	ilBindImage(imageID);

	bool ret = ilLoadImage(nwPath.c_str());
	if (!ret) {
		OWN_LOG("Cannot Load Texture from %s", nwPath.c_str());
		ilDeleteImages(1, &imageID);
		return;
	}
	else{
		ILinfo infoImage;
		iluGetImageInfo(&infoImage);

		ILenum error = ilGetError();
		if (error != IL_NO_ERROR) {
			OWN_LOG("Error getting image info Error:", iluErrorString(error));
		}

		ILuint size;
		ILubyte *data; 
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use 
		size = ilSaveL(IL_DDS, NULL, 0); // gets the size of the data buffer
		if (size != 0) {
			data = new ILubyte[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, data, size) > 0) // Save with the ilSaveIL function
			{
				OWN_LOG("Imported succsfully into DDS");
				
				std::string filename = path;				
				App->fileSys->GetNameFromPath(path, nullptr, &filename, nullptr);
				
				std::string libPath = LIB_TEXTURES_PATH + filename+ ".dds";
				App->fileSys ->writeFile(libPath.c_str(), data, size);
			}
			delete[]data;
			

		}
		data = nullptr;
		ilDeleteImages(1, &imageID);
	}
}