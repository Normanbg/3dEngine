#include "ComponentWindowUI.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "UIPanelGame.h"
#include "GameObject.h"
#include "ModuleResources.h"
#include "ComponentImageUI.h"
#include "ComponentButtonUI.h"

#include "mmgr/mmgr.h"

ComponentWindowUI::ComponentWindowUI()
{
	typeUI = UI_WINDOW;
	type = UI;
}


ComponentWindowUI::~ComponentWindowUI()
{
}

bool ComponentWindowUI::Start()
{
	rectTransform = myGO->GetComponentRectTransform();
	image = myGO->GetComponentImageUI();
	SetResource(App->resources->FindByName(DEFAULT_WIN, Resource::ResType::UI));

	return true;
}

bool ComponentWindowUI::Update()
{
	if (!hasSetToMid) {
		float2 mid = myGO->parent->GetComponentRectTransform()->GetMid();
		rectTransform->SetLocalPos(mid);
		hasSetToMid = true;
	}

	if (draggable && App->gui->isMouseOnGame()) {
		CheckState();
	}
	if (state == WindowState::DRAGGING) {
		if (App->gui->isMouseOnGame())
			DraggWindow();
		else {
			state = WindowState::WINDOW_IDLE;
		}
	}
	return true;
}

void ComponentWindowUI::CleanUp()
{
	image = nullptr;
	if (windImage)
		windImage->CleanUp();
}

void ComponentWindowUI::DraggWindow()
{
	float2 mousePos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
	if (lastPos.x != mousePos.x || mousePos.y != mousePos.y)
	{
		float2 winPos =	(rectTransform->GetGlobalPos())  - (lastPos - mousePos);
		CheckLimits(winPos);
		lastPos = mousePos;
		rectTransform->SetLocalPos(winPos);
	}
}

void ComponentWindowUI::CheckLimits(float2& newPos) {
	if (newPos.x < 0.f)
	{
		newPos.x = 0.f;
	}
	if (newPos.y < 0.f)
	{
		newPos.y = 0.f;
	}
	ComponentRectTransform* parentRect = myGO->parent->GetComponentRectTransform();
	float parentWidth = parentRect->GetWidth();
	float widthMax = (parentWidth - rectTransform->GetWidth());
	if (newPos.x > widthMax)
	{
		newPos.x = widthMax;
	}
	float parentHeight = parentRect->GetHeight();
	float heightMax = (parentHeight  - rectTransform->GetHeight());
	if (newPos.y > heightMax)
	{
		newPos.y = heightMax;
	}
}

void ComponentWindowUI::CheckState() {
	if (IsMouseOver()){
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_IDLE) {
			state = WindowState::WIN_MOUSEOVER;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && CheckChildsState()) {
			state = WindowState::DRAGGING;
			lastPos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && state == WindowState::DRAGGING) {
			state = WindowState::WIN_MOUSEOVER;
		}
	}
	else if (state == WIN_MOUSEOVER) {
		state = WindowState::WINDOW_IDLE;
	}
}

bool ComponentWindowUI::IsMouseOver() {
	float2 mousePos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
	if (rectTransform->GetGlobalPos().x <= mousePos.x && mousePos.x <= rectTransform->GetGlobalPos().x + rectTransform->GetWidth() &&
		rectTransform->GetGlobalPos().y <= mousePos.y && mousePos.y <= rectTransform->GetGlobalPos().y + rectTransform->GetHeight()) {
		return true;
	}
	else
		return false;
}

bool ComponentWindowUI::CheckChildsState()
{
	std::vector <GameObject*>	childrens = myGO->childrens;
	for (auto it : childrens) {
		if (it->GetComponentButtonUI() != nullptr && it->GetComponentButtonUI()->state != ButtonState::IDLE) {
			return false;
		}
		if (it->GetComponentCheckBoxUI() != nullptr && it->GetComponentCheckBoxUI()->IsMouseOver()) {
			return false;
		}
		//....missing to check input
	}
	return true;
}

void ComponentWindowUI::SetResource(uuid resource)
{
	windImage = (ResourceTexture*)App->resources->Get(resource);
	windImage->LoadInMemory();
	 
	if (image != nullptr) {
		image->SetResource(App->resources->FindByName(windImage->GetName(), Resource::ResType::UI));
	}
}

void ComponentWindowUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());

	float windowSize = ImGui::GetWindowContentRegionWidth();
	ImGui::Checkbox("Draggable", &draggable);
	if (ImGui::CollapsingHeader("Background Image")) {
		const char* windMaterial = NULL;
		if (windImage != nullptr) {
			windMaterial = windImage->GetName();
		}
		if (ImGui::BeginCombo("   ", windMaterial))
		{
			std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::UI);

			for (int i = 0; i < mat.size(); i++)
			{
				bool is_selected = false;
				if (windMaterial != nullptr) {
					const char* n = mat[i]->GetName();
					bool is_selected = (strcmp(windMaterial, n) == 0);
				}
				if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
					windMaterial = mat[i]->GetName();
					SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::UI));

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}

				}
			}
			ImGui::EndCombo();
		}
		if (windImage != nullptr) {
			ImGui::Image((void*)(windImage->gpuID), ImVec2(windowSize / 2, windowSize / 2), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size:\n Width: %dpx | Height: %dpx ", windImage->width, windImage->height);
			windMaterial = nullptr;
		}
	}
}

void ComponentWindowUI::DrawUI()
{
}

void ComponentWindowUI::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	alpha = data->GetFloat("Alpha", 1.0f);
	SetResource(App->resources->FindByName(data->GetString("Idle", ""), Resource::ResType::UI));
	hasSetToMid = true;
}

void ComponentWindowUI::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	data.AddFloat("Alpha", alpha);
	if (windImage)
		data.AddString("Idle", windImage->GetName());
}