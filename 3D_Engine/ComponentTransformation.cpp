#include "ComponentTransformation.h"
#include "GameObject.h"



ComponentTransformation::ComponentTransformation()
{
}


ComponentTransformation::~ComponentTransformation()
{
	
}

bool ComponentTransformation::Update(){



	return true;
}

void ComponentTransformation::CleanUp()
{
}

void ComponentTransformation::Test(float3 pos)
{
	if (myGO->parent != nullptr)
	{
		//ROOT MUST BE IN 0
		matrixGlobal = matrixLocal = float4x4::FromTRS(transform.position, transform.rotation, transform.scale);
	}
	else
	{
		//matrixLocal
	}

}


