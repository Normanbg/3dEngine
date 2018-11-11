#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"
#include "ModuleTextures.h"
#include "ComponentWithResource.h"

#include <string>

struct Material;

class ComponentMaterial : public Component, public ComponentWithResource
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	Material* texture = nullptr;
	float3 colors = { 0,0,0 };

	bool Update() override;
	void CleanUp()override;
	void DrawInspector()override;

	void Save(Config& data) const;
	void Load(Config* data);
};

#endif // !__COMPONENTMATERIAL_H__