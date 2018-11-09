#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Application.h"
#include "Module.h"
#include "MathGeoLib/Math/Float3.h"

#include <vector>

struct Material {

	uint texWidth = 0;
	uint texHeight = 0;

	//std::string path;
	std::string name;

	GLuint textureID = -1;
	

	void CleanUp();


};

class ModuleTextures :	public Module {
public:
	ModuleTextures(bool start_enabled = true);
	~ModuleTextures();

	bool Init(JSON_Object* obj) override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	
	void AddMaterial(Material* tex);
	Material* GetMaterialsFromID(GLuint id);
	Material* GetMaterialsFromName(const char* name);
	GLuint CheckIfImageAlreadyLoaded(const char * _path);

	void LoadDroppedTexture(char*droppedFileDire);

	std::vector<Material*> materials;
};

#endif __MODULETEXTURES_H__