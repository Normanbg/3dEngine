#ifndef __MODULETEXTURES_H__
#define __MODULETEXTURES_H__

#include "Application.h"
#include "Module.h"
#include "MathGeoLib\Math/Float3.h"

#include <vector>

struct Material {

	uint texWidth = 0;
	uint texHeight = 0;

	std::string path;

	GLuint textureID = -1;
	float3 colors = { 0,0,0 };

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

	void AddTexture(Material* tex);
	Material* GetMaterialsFromID(GLuint id);
	GLuint CheckIfImageAlreadyLoaded(const char * _path);

	std::vector<Material> materials;
};

#endif __MODULETEXTURES_H__