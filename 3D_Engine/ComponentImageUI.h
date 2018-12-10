#ifndef __COMPONENTIMAGEUI_H__
#define __COMPONENTIMAGEUI_H__

#include "Component.h"
#include "ComponentWithResource.h"
#include "ResourceTexture.h"


class ComponentImageUI :
	public Component, public ComponentWithResource
{
public:
	ComponentImageUI();
	~ComponentImageUI();

	bool Update() override;
	void CleanUp() override;
	void DrawInspector() override;

	const bool HasTexture() const;


	void SetResource(uuid resource)override;
	const uint GetTexID() const;
	Resource* GetResource() const override { return (Resource*)resourceTexture; }

private:
	ResourceTexture* resourceTexture = nullptr;
};


#endif // !__COMPONENTIMAGEUI_H__