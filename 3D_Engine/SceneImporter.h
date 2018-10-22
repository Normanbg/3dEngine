#ifndef __SCENEIMPORTER_H__
#define __SCENEIMPORTER_H__

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

class SceneImporter
{


public:
	SceneImporter();
	~SceneImporter();

public:
	void LoadFBXtoPEI(const char* path);

};

#endif //__SCENEIMPORTER_H__
