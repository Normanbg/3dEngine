#ifndef __TEXTUREIMPORTER_H__
#define __TEXTUREIMPORTER_H__

#include "Application.h"
#include "Globals.h"
#include "ComponentMaterial.h"

struct ComponentMaterial;
class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	void Init();
	bool ImportToDDS(const char * texPath, const char* texName = nullptr, std::vector<std::string>* written = nullptr);

	void LoadDroppedTexture(char * droppedFileDire);
		
	GLuint LoadTexture(const char* path,  uint &texWidth, uint &texHeight);

	bool ImportTexture(const char * tex, std::vector<std::string>* written);

	


public: 
	
	uint imageID = 0;
	
};

#endif //__TEXTUREIMPORTER_H__


