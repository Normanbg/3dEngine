#include "ComponentTransformation.h"
#include "GameObject.h"
#include "ModuleScene.h"

#include <vector>



ComponentTransformation::ComponentTransformation()
{
	type = TRANSFORM;
	globalMatrix.SetIdentity();
}

ComponentTransformation::~ComponentTransformation()
{

}

bool ComponentTransformation::Update() {



	return true;
}

void ComponentTransformation::CleanUp()
{

}

void ComponentTransformation::DrawInspector()
{
	ImGui::Separator();
	float3 _pos = getPos();
	float3 _rot = getEulerRot();
	float3 _scale = getScale();

	GameObject* selectedRecover = App->scene->gObjSelected;
	if (selectedRecover != nullptr)//SETTING THIS BECAUSE WE ONLY ALLOW TO MOVE ONE GAMEOBJECT (ONLY 1 COMP. TRANSFORM DRAWED)-------
	{
		if (ImGui::DragFloat3("Position", (float*)&_pos, 0.1f))
		{
			selectedRecover->transformComp->setPos(_pos);
			///MISSING: RECALC BOUNDING BOX!!!!!!!!!!!!!
		}
		if (ImGui::DragFloat3("Rotation", (float*)&_rot, 0.1f)) {

			selectedRecover->transformComp->setRotEuler(_rot);
			///MISSING: RECALC BOUNDING BOX!!!!!!!!!!!!!
		}
		if (ImGui::DragFloat3("Scale", (float*)&_scale, 0.1f, 0.0f)) {
			
			selectedRecover->transformComp->setScale(_scale);
			///MISSING: RECALC BOUNDING BOX!!!!!!!!!!!!!
		}
		if (ImGui::SmallButton("Reset"))
		{			
			selectedRecover->transformComp->setPos(float3::zero);
			selectedRecover->transformComp->setRotEuler(float3::zero);
			selectedRecover->transformComp->setScale(float3::one);
			///MISSING: RECALC BOUNDING BOX!!!!!!!!!!!!!
		}
	}
	ImGui::Separator();
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

void ComponentTransformation::setLocalMatrix(float4x4 newLocalMat) {
	localMatrix = newLocalMat;
	newLocalMat.Decompose(transform.position, transform.rotationQuat, transform.scale);
	transform.rotEuler = transform.rotationQuat.ToEulerXYZ() * RADTODEG;
}

void ComponentTransformation::UpdateLocalMatrix() {
	localMatrix = float4x4::FromTRS(transform.position, transform.rotationQuat, transform.scale);
}

void ComponentTransformation::Save(Config & data) const
{
	data.AddUInt("UUID", uuid);
	
}

void ComponentTransformation::Load(Config * data)
{
	uuid = data->GetUInt("UUID");
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