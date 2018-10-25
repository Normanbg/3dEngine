#include "UIPanelInspector.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleScene.h"

UIPanelInspector::UIPanelInspector(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelInspector::~UIPanelInspector()
{
}

void UIPanelInspector::Draw() {
	ImGui::Begin(name.c_str(), &active);
	std::vector<Mesh>* meshRecover = App->renderer3D->GetMeshesList(); int i = 0;
	for (std::vector<Mesh>::iterator meshIterator = (*meshRecover).begin(); meshIterator != (*meshRecover).end(); i++, meshIterator++) {
		char meshNumber[30];
		sprintf_s(meshNumber, 30, "%d.%s mesh " , i + 1, meshIterator._Ptr->name.c_str());
		if (ImGui::CollapsingHeader(meshNumber)) {
			ImGui::PushID("Transformation" + i);
			if (ImGui::TreeNode("Transformation")) {
				ImGui::Text("Position:\n X: %0.1f \n Y: %0.1f \n Z: %0.1f", meshIterator._Ptr->position.x, meshIterator._Ptr->position.y, meshIterator._Ptr->position.z);
				
				float3 eulerRot = meshIterator._Ptr->rotation.ToEulerXYZ();//to transform quaternion to Euler angle
				eulerRot *= 180/ pi; // radians to degrees
				ImGui::Text("Rotation:\n X: %0.1f \n Y: %0.1f \n Z: %0.1f", eulerRot.x, eulerRot.y, eulerRot.z);
				ImGui::Text("Scale:\n X: %0.1f \n Y: %0.1f \n Z: %0.1f", meshIterator._Ptr->scale.x, meshIterator._Ptr->scale.y, meshIterator._Ptr->scale.z);
				ImGui::TreePop();
			}
			ImGui::PopID();
			ImGui::PushID("Geometry" + i);
			if (ImGui::TreeNode("Geometry")) {
				ImGui::Text("Vertices: %d", meshIterator._Ptr->num_vertex);
				ImGui::Text("Triangles: %d", meshIterator._Ptr->num_faces);
				ImGui::Text("Indices: %d", meshIterator._Ptr->num_index);
				ImGui::Text("Normals: %d", meshIterator._Ptr->num_normals);
				ImGui::TreePop();
			}
			ImGui::PopID();
			ImGui::PushID("Textures" + i);
			if (ImGui::TreeNode("Textures")) {
				Texture* tex = App->renderer3D->GetTextureFromID((*meshIterator).texID);
				ImGui::Text("Texture size:\n Width: %dpx \n Height: %dpx \n Texture coords: %d", tex->texWidth, tex->texHeight, meshIterator._Ptr->num_textureCoords);
				float windowSize = ImGui::GetWindowContentRegionWidth();
				ImGui::Image((void*)(meshIterator._Ptr->texID), ImVec2(windowSize, windowSize));
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}
	ImGui::End();
}
