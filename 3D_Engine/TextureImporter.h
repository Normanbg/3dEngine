#ifndef __TEXTUREIMPORTER_H__
#define __TEXTUREIMPORTER_H__

#include "Application.h"
#include "Globals.h"


class Mesh;

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	void Init();
	void ImportToDDS(const char * path);
		
	GLuint LoadTexture(const char* path, uint& width, uint& height);

	


public: 
	
	uint imageID = 0;
	
};

#endif //__TEXTUREIMPORTER_H__


