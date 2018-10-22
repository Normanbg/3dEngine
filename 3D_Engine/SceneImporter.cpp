#include "SceneImporter.h"
#include "Globals.h"


SceneImporter::SceneImporter()
{
}


SceneImporter::~SceneImporter()
{
}

void SceneImporter::LoadFBXtoPEI(const char * path)
{
	//NEED TO LOAD FROM /ASSETS
	std::string rootPath = MODELS_PATH;
	rootPath += path;



	// AND EXPORT TO /LIBRARY

}
