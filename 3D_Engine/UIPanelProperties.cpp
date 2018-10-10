#include "UIPanelProperties.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"

UIPanelProperties::UIPanelProperties(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelProperties::~UIPanelProperties()
{
}

void UIPanelProperties::Draw() {
	ImGui::Begin(name.c_str(), &active);
	std::vector<Mesh>* meshRecover = App->renderer3D->GetMeshesList(); int i = 0;
	for (std::vector<Mesh>::iterator meshIterator = (*meshRecover).begin(); meshIterator != (*meshRecover).end(); i++, meshIterator++) {
		char meshNumber[30];
		sprintf_s(meshNumber, 30, "%d.%s mesh " , i + 1, meshIterator._Ptr->name.c_str());
		if (ImGui::CollapsingHeader(meshNumber)) {
			ImGui::PushID("Transformation" + i);
			if (ImGui::TreeNode("Transformation")) {
				ImGui::Text("Position:\n X: %0.1f \n Y: %0.1f \n Z: %0.1f", 0.0f, 0.0f, 0.0f);
				ImGui::Text("Rotation:\n X: %0.1f \n Y: %0.1f \n Z: %0.1f", 0.0f, 0.0f, 0.0f);
				ImGui::Text("Scale:\n X: %0.1f \n Y: %0.1f \n Z: %0.1f", 1.0f, 1.0f, 1.0f);
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
				ImGui::Text("Texture size:\n Width: %dpx \n Height: %dpx \n Texture coords: %d", meshIterator._Ptr->texWidth, meshIterator._Ptr->texHeight, meshIterator._Ptr->num_textureCoords);
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}
	ImGui::End();
}
