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

void ComponentTransformation::DrawInspector()
{
	float3 _pos = getPos();
	float3 _rot = getEulerRot();
	float3 _scale = getScale();

	if (ImGui::DragFloat3("Position", (float*)&_pos, 0.1f))
		setPos(_pos);

	if (ImGui::DragFloat3("Rotation", (float*)&_rot, 0.1f))
		setRotEuler(_rot);

	if (ImGui::DragFloat3("Scale", (float*)&_scale, 0.1f, 0.0f))
		setScale(_scale);
	if (ImGui::SmallButton("Reset"))
	{
			setPos(float3::zero);
			setRotEuler(float3::zero);
			setScale(float3::one);
	}
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

float3 ComponentTransformation::getScale()
{
	return transform.scale;
}

float3 ComponentTransformation::getEulerRot()
{
	return transform.rotEuler;
}

Quat ComponentTransformation::getQuatRot()
{
	return transform.rotationQuat;
}
