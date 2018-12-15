#ifndef __COMPONENTIMAGEUI_H__
#define __COMPONENTIMAGEUI_H__

#include "Component.h"
#include "ComponentWithResource.h"
#include "ResourceTexture.h"
#include "ComponentRectTransform.h"
#include "ComponentUI.h"

class ComponentImageUI : public ComponentUI, public ComponentWithResource, public Component
{
public:
	ComponentImageUI();
	~ComponentImageUI();

	bool Start() override;
	bool Update() override;
	void CleanUp() override;

	inline void doStart() override { Start(); }
	inline void doUpdate() override { Update(); }
	inline void doCleanUp() override { CleanUp(); }

	void DrawInspector() override;

	void DrawUI() override;
	void GenBuffer();

	const bool HasTexture() const;


	void SetResource(uuid resource)override;
	const uint GetTexID() const;
	Resource* GetResource() const override { return (Resource*)resourceTexture; }

private:
	float2* texCoords = nullptr;
	ResourceTexture* resourceTexture = nullptr;
	ComponentRectTransform* rectTransform = nullptr;
};


#endif // !__COMPONENTIMAGEUI_H__