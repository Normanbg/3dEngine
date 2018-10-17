#ifndef __COMPONENTMATERIAL_H__
#define __COMPONENTMATERIAL_H__

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	bool Update() override;


	void CleanUp()override;
};

#endif // !__COMPONENTMATERIAL_H__