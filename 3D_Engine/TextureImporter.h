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
	void DrawTexture();

	void LoadCheckeredPlane();
	void DrawCheckeredPlane();

public: 
	
	uint imageID = 0;
};

#endif //__TEXTUREIMPORTER_H__



