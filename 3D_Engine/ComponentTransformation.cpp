#include "ComponentTransformation.h"
#include "GameObject.h"
#include "ModuleScene.h"

#include <vector>

#include "mmgr/mmgr.h"


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
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	float3 _pos = getPos();
	float3 _rot = getEulerRot();
	float3 _scale = getScale();

	GameObject* selectedRecover = App->scene->gObjSelected;
	//bool moved = false;
	if (selectedRecover != nullptr)//SETTING THIS BECAUSE WE ONLY ALLOW TO MOVE ONE GAMEOBJECT (ONLY 1 COMP. TRANSFORM DRAWED)-------
	{
		if (ImGui::DragFloat3("Position", (float*)&_pos, 0.1f))
		{
			selectedRecover->GetComponentTransform()->setPos(_pos);
			if (selectedRecover->staticGO)
				App->scene->objectMoved = true;
		}
		if (ImGui::DragFloat3("Rotation", (float*)&_rot, 0.1f)) {

			selectedRecover->GetComponentTransform()->setRotEuler(_rot);
			if (selectedRecover->staticGO)
				App->scene->objectMoved = true;
		}
		if (ImGui::DragFloat3("Scale", (float*)&_scale, 0.1f, 0.0f)) {
			
			selectedRecover->GetComponentTransform()->setScale(_scale);
			if (selectedRecover->staticGO)
				App->scene->objectMoved = true;
		}
		if (ImGui::SmallButton("Reset"))
		{			
			selectedRecover->GetComponentTransform()->setPos(float3::zero);
			selectedRecover->GetComponentTransform()->setRotEuler(float3::zero);
			selectedRecover->GetComponentTransform()->setScale(float3::one);
			if (selectedRecover->staticGO)
				App->scene->objectMoved = true;
		}
	}
	ImGui::Separator();
}

void ComponentTransformation::setGlobalMatrix(const float4x4 newGlobalMat)
{
	globalMatrix = newGlobalMat;
	if (myGO->parent != nullptr)
	{
		ComponentTransformation* parentTrans = myGO->parent->GetComponentTransform();
		float4x4 newlocalMatrix = parentTrans->getGlobalMatrix().Inverted() * globalMatrix;
		setLocalMatrix(newlocalMatrix);
	}
}

void ComponentTransformation::setLocalMatrix(const float4x4 newLocalMat) {
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

	data.AddFloat3("Translation", getPos());
	data.AddFloat3("Scale", getScale());
	data.AddFloat3("Rotation", getEulerRot()); //save rotation as eulerangle(float3) to save memory.
	
}

void ComponentTransformation::Load(Config * data)
{
	UUID = data->GetUInt("UUID");

	setPos(data->GetFloat3("Translation", { 0,0,0 }));
	setRotEuler(data->GetFloat3("Rotation", { 0,0,0 }));
	setScale(data->GetFloat3("Scale", { 0,0,0 }));
}

void ComponentTransformation::setPos(const float3 _newpos)
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

void ComponentTransformation::setRotQuat(const Quat qNewRot)
{
	transform.rotationQuat = qNewRot;
	transform.rotEuler = transform.rotationQuat.ToEulerXYZ() * RADTODEG;
	UpdateLocalMatrix();
}

void ComponentTransformation::setRotEuler(const float3 _newrot)
{
	transform.rotEuler = _newrot;
	transform.rotationQuat = Quat::FromEulerXYZ(transform.rotEuler.x * DEGTORAD, transform.rotEuler.y * DEGTORAD, transform.rotEuler.z * DEGTORAD);
	UpdateLocalMatrix();
}

const float3 ComponentTransformation::getPos() const
{
	return transform.position;
}

const float3 ComponentTransformation::getScale() const
{
	return transform.scale;
}

const float3 ComponentTransformation::getEulerRot() const
{
	return transform.rotEuler;
}

const Quat ComponentTransformation::getQuatRot() const
{
	return transform.rotationQuat;
}

const float4x4 ComponentTransformation::getGlobalMatrix() const
{
	return globalMatrix;
}

const float4x4 ComponentTransformation::getLocalMatrix() const
{
	return localMatrix;
}
