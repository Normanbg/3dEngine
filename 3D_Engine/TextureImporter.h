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
	
		
	GLuint LoadTexture(const char* path, uint& width, uint& height);

	
	void LoadCheckeredPlane(); // for debug test
	void DrawCheckeredPlane()const ;

public: 
	
	uint imageID = 0;
	
};

#endif //__TEXTUREIMPORTER_H__



