#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#define ROOT_PATH "Assets/"
class Importer
{
public:
	Importer();
	~Importer();

	void InitDebugLog();
	void EndDebugLog();
	
	void LoadFBX(char* path );
	void LoadFBXfromDrop(char* path);
private:
	void LoadFromMesh(aiMesh* mesh);

private:
	char* droppedFile = nullptr;
};

#endif //__IMPORTER_H__



