#ifndef __IMPORTER_H
#define __IMPORTER_H

#include "Modulerenderer3D.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")
class Importer
{
public:
	Importer();
	~Importer();

	void InitDebugLog();
	void EndDebugLog();
	
	void LoadFBX(char* path );
private:
	void LoadFromMesh(aiMesh* mesh);
};



#endif // !__IMPORTER_H
