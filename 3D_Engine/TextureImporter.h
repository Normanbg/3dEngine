#ifndef __TEXTUREIMPORTER_H__
#define __TEXTUREIMPORTER_H__

#include "Application.h"
#include "Globals.h"

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	void Init();
	void CleanUp();

	void LoadTexture(char* path);

public: 
	
};

#endif //__TEXTUREIMPORTER_H__



