#ifndef __MESHIMPORTER_H__
#define __MESHIMPORTER_H__

#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#define ROOT_PATH "Assets/"
class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	void InitDebugLog();
	void EndDebugLog();

	void LoadFBX(char* path );
	void LoadFBXfromDrop(const char* path);
	void ChangeMeshTexture(const char* path);

private:
	void LoadFromMesh(const aiScene* currSc, aiMesh* mesh);

private:
	char* droppedFile = nullptr;
};

#endif //__MESHIMPORTER_H__



