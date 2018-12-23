#include "UIPanelHierarchy.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "ModuleInput.h"

#include "mmgr/mmgr.h"

UIPanelHierarchy::UIPanelHierarchy(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelHierarchy::~UIPanelHierarchy()
{
}

void UIPanelHierarchy::Draw()
{
	ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Game Object")) {
				App->scene->AddGameObject("newGameObject");
			}
			if (ImGui::TreeNode("UI")) {				
				if (ImGui::MenuItem("Image")) {
					//check if canvas created
					GameObject* canvas= App->scene->GetFirstGameObjectCanvas();
					if (canvas == nullptr) {
						canvas = App->scene->AddUIGameObject("Canvas", App->scene->root);
						canvas->AddComponent(CANVAS);
					}
					GameObject* image = App->scene->AddUIGameObject("UI_Image",canvas);
					image->AddUIComponent(UI_IMAGE);
					image = nullptr;
					canvas = nullptr;
				}
				if (ImGui::MenuItem("Text")) {
					//check if canvas created
					GameObject* canvas = App->scene->GetFirstGameObjectCanvas();
					if (canvas == nullptr) {
						canvas = App->scene->AddUIGameObject("Canvas", App->scene->root);
						canvas->AddComponent(CANVAS);
					}
					GameObject* txt = App->scene->AddUIGameObject("UI_Text", canvas);
					txt->AddUIComponent(UI_TEXT);
					txt = nullptr;
					canvas = nullptr;
				}
				if (ImGui::MenuItem("Input")) {
					//check if canvas created
					GameObject* canvas = App->scene->GetFirstGameObjectCanvas();
					if (canvas == nullptr) {
						canvas = App->scene->AddUIGameObject("Canvas", App->scene->root);
						canvas->AddComponent(CANVAS);
					}
					GameObject* inp = App->scene->AddUIGameObject("UI_Input", canvas);					
					inp->AddUIComponent(UI_TEXT);
					inp->AddUIComponent(UI_BUTTON);
					inp->AddUIComponent(UI_INPUT);
					inp = nullptr;
					canvas = nullptr;
				}
				if (ImGui::MenuItem("Button")) {
					//check if canvas created
					GameObject* canvas = App->scene->GetFirstGameObjectCanvas();
					if (canvas == nullptr) {
						canvas = App->scene->AddUIGameObject("Canvas", App->scene->root);
						canvas->AddComponent(CANVAS);
					}
					GameObject* but = App->scene->AddUIGameObject("UI_Button", canvas);
					but->AddUIComponent(UI_BUTTON);
					but = nullptr;
					canvas = nullptr;
				}
				if (ImGui::MenuItem("Window")) {
					//check if canvas created
					GameObject* canvas = App->scene->GetFirstGameObjectCanvas();
					if (canvas == nullptr) {
						canvas = App->scene->AddUIGameObject("Canvas", App->scene->root);
						canvas->AddComponent(CANVAS);
					}
					GameObject* window = App->scene->AddUIGameObject("UI_Window", canvas);
					window->AddUIComponent(UI_WINDOW);
					window = nullptr;
					canvas = nullptr;
				}
				ImGui::TreePop();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (showPopUp) {
		ImGui::OpenPopup("Options");
		if (ImGui::BeginPopup("Options"))
		{
			if (App->scene->gObjSelected != nullptr && App->scene->gObjSelected->GetComponentRectTransform() != nullptr) {
				if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered()) {
					showPopUp = false;
				}
				if (ImGui::MenuItem("Image")) {
					GameObject* img;
					img = App->scene->AddUIGameObject("UI Image", App->scene->gObjSelected);
					img->AddUIComponent(UI_IMAGE);
					showPopUp = false;
				}
				if (ImGui::MenuItem("Button")) {
					GameObject* butt;
					butt = App->scene->AddUIGameObject("UI Button", App->scene->gObjSelected);
					butt->AddUIComponent(UI_BUTTON);
					showPopUp = false;
				}
				if (ImGui::MenuItem("Text")) {
					GameObject* text;
					text = App->scene->AddUIGameObject("UI Text", App->scene->gObjSelected);
					text->AddUIComponent(UI_TEXT);
					showPopUp = false;
				}
				if (ImGui::MenuItem("Input Text")) {
					GameObject* inpText;
					inpText = App->scene->AddUIGameObject("UI Input Text", App->scene->gObjSelected);
					inpText->AddUIComponent(UI_INPUT);
					showPopUp = false;
				}
				if (ImGui::MenuItem("Checkbox")) {
					GameObject* checkB;
					checkB = App->scene->AddUIGameObject("UI Checkbox", App->scene->gObjSelected);
					checkB->AddUIComponent(UI_CHECKBOX);
					showPopUp = false;
				}
				if (ImGui::MenuItem("Window")) {
					GameObject* window;
					window = App->scene->AddUIGameObject("UI Window", App->scene->gObjSelected);
					window->AddUIComponent(UI_WINDOW);
					showPopUp = false;
				}
			}
			else
			{
				showPopUp = false;
			}

			ImGui::EndPopup();
		}

	}

	//FOR DESELECTING GAME OBJECTS
	if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsMouseHoveringWindow()) {
		App->scene->DeselectAll();
	}
	DrawChilds(App->scene->root->childrens);
	ImGui::End();
}

void UIPanelHierarchy::DrawChilds(std::vector<GameObject*> childs){
	for (std::vector<GameObject*>::iterator goIterator = childs.begin(); goIterator != childs.end(); goIterator++)
	{
		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if ((*goIterator) == App->scene->gObjSelected)
			flags |= ImGuiTreeNodeFlags_Selected;
		if ((*goIterator)->childrens.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

		//TODO::COLOR IF IS ACTIVE--------------

		ImGui::PushID((*goIterator)->GetUUID());
		bool treeNodeOpened = ImGui::TreeNodeEx((*goIterator)->name.c_str(), flags);
		ImGui::PopID();
		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1) /*|| (*goIterator) != App->scene->gObjSelected*/)
		{
			App->scene->DeselectAll();
			App->scene->ShowGameObjectInspector((*goIterator));
		}
		if (ImGui::IsItemClicked(1))
			showPopUp = true;

		if (treeNodeOpened)
		{
			DrawChilds((*goIterator)->childrens);
			ImGui::TreePop();
		}
	}
}


