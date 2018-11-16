#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "TextureImporter.h"
#include "ModuleRenderer3D.h"
#include "ModuleFileSystem.h"
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#include "DevIL/include/ilu.h"
#include "mmgr/mmgr.h"


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


GLuint TextureImporter::LoadTexture(const char * path,uint &texWidth,uint &texHeight)
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

bool TextureImporter::ImportTexture(const char* tex, std::vector<std::string>* written)
{
	bool ret = false;

	std::string extension;
	App->fileSys->GetNameFromPath(tex, nullptr, nullptr, nullptr, &extension);
	if (extension != DDS_FORMAT)
		ret = App->texImporter->ImportToDDS(tex, nullptr, written);
	else
		ret = App->fileSys->CopyDDStoLib(tex, written);

	return ret;
}


bool TextureImporter::ImportToDDS( const char* texPath, const char* texName, std::vector<std::string>* written ) {

	OWN_LOG("Importing texture from %s", texPath);
	ILuint imageID;
	

	std::string textureName;
	std::string extension;
	if (texName == nullptr) {
		App->fileSys->GetNameFromPath(texPath, nullptr, &textureName, nullptr, &extension);
	}
	else {
		textureName = texName;
		App->fileSys->GetNameFromPath(texPath, nullptr, nullptr, nullptr, &extension);
	}
	
	

	ilGenImages(1, &imageID); // generates an image
	ilBindImage(imageID);

	bool ret = ilLoadImage(texPath);
	if (!ret) {
		OWN_LOG("Cannot Load Texture from %s", texPath);
		ilDeleteImages(1, &imageID);		
	}
	else{
		
		ILuint size;
		ILubyte *data;		
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use 
		size = ilSaveL(IL_DDS, NULL, 0); // gets the size of the data buffer
		if (size != 0) {
			data = new ILubyte[size]; // allocate data buffer
			if (ilSaveL(IL_DDS, data, size) > 0) // Save with the ilSaveIL function
			{
				OWN_LOG("Imported succsfully into DDS");
				
				std::string uuid;
				
				std::string libPath =   LIB_TEXTURES_PATH + uuid + textureName + DDS_FORMAT;
				if (written) { (*written).push_back(libPath); }
				App->fileSys ->writeFile(libPath.c_str(), data, size);
				ret = true;
			}
			delete[]data;
			

		}
		data = nullptr;
		ilDeleteImages(1, &imageID);		
	}
	return ret;
}

void TextureImporter::LoadDroppedTexture(char * droppedFileDire)
{


	std::string texName;
	std::string extension;

	App->fileSys->GetNameFromPath(droppedFileDire, nullptr, &texName, nullptr, nullptr);
	App->fileSys->GetNameFromPath(droppedFileDire, nullptr, nullptr, nullptr, &extension);
	if (extension != DDS_FORMAT) {
		App->texImporter->ImportToDDS(droppedFileDire, texName.c_str());
	}

	//if (App->textures->CheckIfImageAlreadyLoaded(texName.c_str()) == -1) {
	//	Material* mat = new Material;
	//	if (extension == DDS_FORMAT) {
	//		mat->textureID = App->renderer3D->texImporter->LoadTexture(droppedFileDire, mat);
	//	}
	//	else {
	//		std::string texPath;

	//		texPath = LIB_TEXTURES_PATH + texName + DDS_FORMAT;
	//		mat->textureID = App->renderer3D->texImporter->LoadTexture(texPath.c_str(), mat);
	//	}
	//	mat->name = texName;
	//	App->textures->AddMaterial(mat);
	//}
	else {
		OWN_LOG("Material already loeaded");
	}
}