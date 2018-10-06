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
	void CleanUp();

	GLuint LoadTexture(const char* path);
	void DrawTexture(Mesh* mesh);

	void LoadCheckeredPlane();
	void DrawCheckeredPlane();

public: 
	
	uint imageID = 0;
};

#endif //__TEXTUREIMPORTER_H__



