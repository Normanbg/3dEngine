#ifndef __COMPONENTTRANS_H__
#define __COMPONENTTRANS_H__

#include "Component.h"

class ComponentTransformation :	public Component
{
public:
	ComponentTransformation();
	~ComponentTransformation();

	bool Update() override;

	void CleanUp() override;
};

#endif // !__COMPONENTTRANS_H__
