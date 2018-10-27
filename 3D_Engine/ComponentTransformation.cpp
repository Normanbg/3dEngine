#include "ComponentTransformation.h"
#include "GameObject.h"



ComponentTransformation::ComponentTransformation()
{
	type = TRANSFORM;

	globalMatrix.SetIdentity();

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

void ComponentTransformation::setGlobalMatrix(float4x4 newGlobalMat)
{
	globalMatrix = newGlobalMat;
	if (myGO->parent != nullptr)	
	{
		ComponentTransformation* parentTrans = myGO->GetTransformComponent();
		float4x4 newlocalMatrix = parentTrans->globalMatrix * globalMatrix;
		setLocalMatrix(newlocalMatrix);
	}
}

void ComponentTransformation::setLocalMatrix(float4x4 newLocalMat){
	localMatrix = newLocalMat;
	newLocalMat.Decompose(transform.position, transform.rotationQuat, transform.scale);
	transform.rotEuler = transform.rotationQuat.ToEulerXYZ() * RADTODEG;
}

void ComponentTransformation::UpdateLocalMatrix(){
	localMatrix = float4x4::FromTRS(transform.position, transform.rotationQuat, transform.scale);
}

void ComponentTransformation::setPos(float3 _newpos)
{
	transform.position = _newpos;
	UpdateLocalMatrix();
}

void ComponentTransformation::setScale(float3 _newscale)
{
	if (_newscale.x < 0)
		_newscale.x = 0.001f;
	if (_newscale.y < 0)
		_newscale.y = 0.001f;
	if (_newscale.z < 0)
		_newscale.z = 0.001f;

	transform.scale = _newscale;
	UpdateLocalMatrix();
}

void ComponentTransformation::setRotQuat(Quat qNewRot)
{
	transform.rotationQuat = qNewRot;
	transform.rotEuler = transform.rotationQuat.ToEulerXYZ() * RADTODEG;
	UpdateLocalMatrix();
}

void ComponentTransformation::setRotEuler(float3 _newrot)
{
	transform.rotEuler = _newrot;
	transform.rotationQuat = Quat::FromEulerXYZ(transform.rotEuler.x * DEGTORAD, transform.rotEuler.y * DEGTORAD, transform.rotEuler.z * DEGTORAD);
	UpdateLocalMatrix();
}

float3 ComponentTransformation::getPos()
{
	return transform.position;
}
