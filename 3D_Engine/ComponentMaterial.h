#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"
#include "Globals.h"
#include "ComponentWithResource.h"
#include "ResourceTexture.h"

#include <string>

struct Material;

class ComponentMaterial : public Component, public ComponentWithResource
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	//Material* texture = nullptr;
	float3 colors = { 0,0,0 };

	bool Update() override;
	void CleanUp()override;
	void DrawInspector()override;

	void SetResource(uuid resource)override;
	Resource* GetResource() const override { return (Resource*)resourceTexture; }
	const uint GetTexID() const;
	const char* GetTextureName() const;
	const float GetAlphaTest() const { return alphaTest; }
	const bool HasTexture() const;


	void Save(Config& data) const;
	void Load(Config* data);

	bool doBlendTest = false;

private:
	ResourceTexture* resourceTexture = nullptr;
	float alphaTest = 1.0f;
	

};

#endif // !__COMPONENTMATERIAL_H__