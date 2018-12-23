#include "ComponentTransformation.h"
#include "GameObject.h"
#include "ModuleScene.h"

#include <vector>

#include "mmgr/mmgr.h"


ComponentTransformation::ComponentTransformation()
{
	type = TRANSFORM;
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
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	float3 _pos = GetPos();
	float3 _rot = GetEulerRot();
	float3 _scale = GetScale();

	GameObject* selectedRecover = App->scene->gObjSelected;
	//bool moved = false;
	if (selectedRecover != nullptr)//SETTING THIS BECAUSE WE ONLY ALLOW TO MOVE ONE GAMEOBJECT (ONLY 1 COMP. TRANSFORM DRAWED)-------
	{
		if (ImGui::DragFloat3("Position", (float*)&_pos, 0.1f))
		{
			selectedRecover->GetComponentTransform()->SetPos(_pos);
			if (selectedRecover->staticGO)
				App->scene->objectMoved = true;
		}
		if (ImGui::DragFloat3("Rotation", (float*)&_rot, 0.1f)) {

			selectedRecover->GetComponentTransform()->SetRotEuler(_rot);
			if (selectedRecover->staticGO)
				App->scene->objectMoved = true;
		}
		if (ImGui::DragFloat3("Scale", (float*)&_scale, 0.1f, 0.0f)) {
			
			selectedRecover->GetComponentTransform()->SetScale(_scale);
			if (selectedRecover->staticGO)
				App->scene->objectMoved = true;
		}
		if (ImGui::SmallButton("Reset"))
		{			
			selectedRecover->GetComponentTransform()->SetPos(float3::zero);
			selectedRecover->GetComponentTransform()->SetRotEuler(float3::zero);
			selectedRecover->GetComponentTransform()->SetScale(float3::one);
			if (selectedRecover->staticGO)
				App->scene->objectMoved = true;
		}
	}
	ImGui::Separator();
}

void ComponentTransformation::setGlobalMatrix(float4x4 newGlobalMat)
{
	globalMatrix = newGlobalMat;
	if (myGO->parent != nullptr)
	{
		ComponentTransformation* parentTrans = myGO->parent->GetComponentTransform();
		float4x4 newlocalMatrix = parentTrans->GetGlobalMatrix().Inverted() * globalMatrix;
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
	data.AddUInt("UUID", UUID);
	data.AddFloat3("Translation", GetPos());
	data.AddFloat3("Scale", GetScale());
	data.AddFloat3("Rotation", GetEulerRot()); //save rotation as eulerangle(float3) to save memory.
	
}

void ComponentTransformation::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	SetPos(data->GetFloat3("Translation", { 0,0,0 }));
	SetRotEuler(data->GetFloat3("Rotation", { 0,0,0 }));
	SetScale(data->GetFloat3("Scale", { 0,0,0 }));
}

void ComponentTransformation::SetPos( float3 _newpos)
{
	transform.position = _newpos;
	UpdateLocalMatrix();
}

void ComponentTransformation::SetScale(float3 _newscale)
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

void ComponentTransformation::SetRotQuat( Quat qNewRot)
{
	transform.rotationQuat = qNewRot;
	transform.rotEuler = transform.rotationQuat.ToEulerXYZ() * RADTODEG;
	UpdateLocalMatrix();
}

void ComponentTransformation::SetRotEuler( float3 _newrot)
{
	transform.rotEuler = _newrot;
	transform.rotationQuat = Quat::FromEulerXYZ(transform.rotEuler.x * DEGTORAD, transform.rotEuler.y * DEGTORAD, transform.rotEuler.z * DEGTORAD);
	UpdateLocalMatrix();
}

const float3 ComponentTransformation::GetPos() const
{
	return transform.position;
}

const float3 ComponentTransformation::GetScale() const
{
	return transform.scale;
}

const float3 ComponentTransformation::GetEulerRot() const
{
	return transform.rotEuler;
}

const Quat ComponentTransformation::GetQuatRot() const
{
	return transform.rotationQuat;
}

const float4x4 ComponentTransformation::GetGlobalMatrix() const
{
	return globalMatrix;
}

const float4x4 ComponentTransformation::GetLocalMatrix() const
{
	return localMatrix;
}
