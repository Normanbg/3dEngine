#ifndef __IMPORTER_H
#define __IMPORTER_H

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

	void Log();
};



#endif // !__IMPORTER_H
