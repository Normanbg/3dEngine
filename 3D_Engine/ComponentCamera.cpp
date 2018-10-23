#include "ComponentCamera.h"



ComponentCamera::ComponentCamera()
{
	X = vec(1.0f, 0.0f, 0.0f);
	Y = vec(0.0f, 1.0f, 0.0f);
	Z = vec(0.0f, 0.0f, 1.0f);

	//position = transform.position
	Reference = vec(0.0f, 0.0f, 0.0f);

	LookAt(Reference);
}


ComponentCamera::~ComponentCamera()
{
}


bool ComponentCamera::Update()
{
	
	return true;
}

void ComponentCamera::CleanUp()
{
}
