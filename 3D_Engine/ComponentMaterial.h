#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"
#include "ModuleTextures.h"
#include "SceneImporter.h"//
#include "TextureImporter.h"//


class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	Material* texture;


	bool Update() override;
	void CleanUp()override;
	void DrawInspector()override;
};

#endif // !__COMPONENTMATERIAL_H__