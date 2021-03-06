#include "ComponentButtonUI.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "UIPanelGame.h"
#include "GameObject.h"
#include "ModuleResources.h"
#include "ComponentImageUI.h"

#include "mmgr/mmgr.h"

ComponentButtonUI::ComponentButtonUI()
{
	typeUI = UI_BUTTON;
	type = UI;
}


ComponentButtonUI::~ComponentButtonUI()
{
}

void ComponentButtonUI::UpdateRectTransform()
{
}

void ComponentButtonUI::DrawUI()
{
}

bool ComponentButtonUI::Start()
{
	image = myGO->GetComponentImageUI();
	rectTransform = myGO->GetComponentRectTransform();
	rectTransform->SetWidth(60);
	rectTransform->SetHeight(20);
	SetResource(App->resources->FindByName(DEFAULT_IDLE_BT, Resource::ResType::UI), 0);
	SetResource(App->resources->FindByName(DEFAULT_HOVER_BT, Resource::ResType::UI), 1);
	SetResource(App->resources->FindByName(DEFAULT_PRESSED_BT, Resource::ResType::UI), 2);
	return true;
}

bool ComponentButtonUI::Update()
{
	if (!hasSetToMid) {
		float2 mid = myGO->parent->GetComponentRectTransform()->GetMid();
		rectTransform->SetLocalPos(mid);
		hasSetToMid = true;
	}

	GameObject* canvasGO = App->scene->GetFirstGameObjectCanvas();
	if (App->gui->isMouseOnGame() && myGO != canvasGO)
		CheckState();
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state == PRESSED)
	{
		state = IDLE;
		if (idleImg != nullptr)
			ChangeGOImage();
	}
	
	if (fadingOut) {
		FadeOut();
	}
	if (fadingIn) {
		FadeIn();
	}
	image->SetAlpha(alpha);
	return true;
}

void ComponentButtonUI::CleanUp()
{
	if (HasTexture(idleImg)) {
		idleImg->CleanUp();
	}
	if (HasTexture(pressedImg)) {
		pressedImg->CleanUp();
	}
	if (HasTexture(hoverImg)) {
		hoverImg->CleanUp();
	}

	rectTransform = nullptr;
	idleImg = nullptr;
	hoverImg = nullptr;
	pressedImg = nullptr;
	myGO = nullptr;
	image = nullptr;
}

void ComponentButtonUI::CheckState() {	
	if (IsMouseOver()){
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state == PRESSED) {
			state = MOUSEOVER;
			if (hoverImg != nullptr)
				ChangeGOImage();
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state != PRESSED) {
			state = PRESSED;
			PressedCallback();
			if (pressedImg != nullptr)
				ChangeGOImage();
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_IDLE && state != PRESSED) {
			state = MOUSEOVER;
			if (hoverImg != nullptr)
				ChangeGOImage();
		}
	}
	else if (state == MOUSEOVER){
		state = IDLE;
		if (idleImg != nullptr)
			ChangeGOImage();
	}
	else if (state == PRESSED && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {
		state = IDLE;
		if (idleImg != nullptr)
			ChangeGOImage();
		

	}
}

bool ComponentButtonUI::IsMouseOver() {
	float2 mousePos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
	if (rectTransform->GetGlobalPos().x <= mousePos.x && mousePos.x <= rectTransform->GetGlobalPos().x + rectTransform->GetWidth() &&
		rectTransform->GetGlobalPos().y <= mousePos.y && mousePos.y <= rectTransform->GetGlobalPos().y + rectTransform->GetHeight()) {
		return true;
	}
	else
		return false;
}

void ComponentButtonUI::PressedCallback()
{
	App->scene->functions->ExecuteFunction(function);
}

void ComponentButtonUI::ChangeGOImage()
{
	
	switch (state)
	{
	case ButtonState::IDLE:
		image->SetResource(App->resources->FindByName(idleImg->GetName(), Resource::ResType::UI));
		break;
	case ButtonState::MOUSEOVER:
		image->SetResource(App->resources->FindByName(hoverImg->GetName(), Resource::ResType::UI));
		break;
	case ButtonState::PRESSED:
		image->SetResource(App->resources->FindByName(pressedImg->GetName(), Resource::ResType::UI));
		break;
	default:
		break;
	}
}

void ComponentButtonUI::FadeIn()
{
	alpha += DELTA_ALPHA;
	if (alpha >= 1.0f) {
		fadingIn = false;
		alpha = 1.0f;
	}	
}

void ComponentButtonUI::FadeOut()
{
	alpha -= DELTA_ALPHA;
	if (alpha <= 0.0f) {
		fadingOut = false;
		alpha = 0.0f;
	}
}

void ComponentButtonUI::SetResource(uuid resource, int numRes)
{
	if (resource != 0) {
		switch (numRes)
		{
		case 0:
			idleImg = (ResourceTexture*)App->resources->Get(resource);
			idleImg->LoadInMemory();
			ChangeGOImage();
			break;
		case 1:
			hoverImg = (ResourceTexture*)App->resources->Get(resource);
			hoverImg->LoadInMemory();
			break;
		case 2:
			pressedImg = (ResourceTexture*)App->resources->Get(resource);
			pressedImg->LoadInMemory();
			break;
		}
	}
}

const bool ComponentButtonUI::HasTexture(ResourceTexture* res) const
{
	bool ret;
	res ? ret = true : ret = false;
	return ret;
}

void ComponentButtonUI::Load(Config * data)
{
	UUID = data->GetUInt("UUID", 0);
	alpha = data->GetFloat("Alpha", 1.0f);
	SetResource(App->resources->FindByName(data->GetString("Idle", ""), Resource::ResType::UI),0);
	SetResource(App->resources->FindByName(data->GetString("Hover", ""), Resource::ResType::UI), 1);
	SetResource(App->resources->FindByName(data->GetString("Press", ""), Resource::ResType::UI), 2);
	function =(Functions) data->GetInt("Function", 0);
	hasSetToMid = true;
}

void ComponentButtonUI::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	data.AddFloat("Alpha", alpha);
	if(idleImg)
	data.AddString("Idle",idleImg->GetName());
	if (hoverImg)
	data.AddString("Hover", hoverImg->GetName());
	if (pressedImg)
	data.AddString("Press", pressedImg->GetName());
	data.AddInt("Function", function);
}

void ComponentButtonUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());

	float windowSize = ImGui::GetWindowContentRegionWidth();
	ImGui::SliderFloat("Alpha", &alpha, 0.0f, 1.0f);
	if (ImGui::Button("FadeIn")) { 
		alpha = 0.0f; fadingIn = true; }
	ImGui::SameLine();
	if(ImGui::Button("FadeOut")) { 
		alpha = 1.0f; fadingOut = true; }

	if (ImGui::CollapsingHeader("Idle Image")) {
		const char* idleMaterial = NULL;
		if (idleImg != nullptr) {
			idleMaterial = idleImg->GetName();
		}
		if (ImGui::BeginCombo("   ", idleMaterial))
		{
			std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::UI);

			for (int i = 0; i < mat.size(); i++)
			{
				bool is_selected = false;
				if (idleMaterial != nullptr) {
					const char* n = mat[i]->GetName();
					bool is_selected = (strcmp(idleMaterial, n) == 0);
				}
				if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
					idleMaterial = mat[i]->GetName();
					SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::UI), 0);

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}

				}

			}
			ImGui::EndCombo();
		}
		if (idleImg != nullptr) {
			ImGui::Image((void*)(idleImg->gpuID), ImVec2(windowSize / 2, windowSize / 2), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size:\n Width: %dpx | Height: %dpx ", idleImg->width, idleImg->height);
			idleMaterial = nullptr;
		}
	}
	
	if (ImGui::CollapsingHeader("Hover Image")) {
		const char* hoverMaterial = NULL;
		if (hoverImg != nullptr) {
			hoverMaterial = hoverImg->GetName();
		}
		if (ImGui::BeginCombo(" ", hoverMaterial))
		{
			std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::UI);

			for (int i = 0; i < mat.size(); i++)
			{
				bool is_selected = false;
				if (hoverMaterial != nullptr) {
					const char* n = mat[i]->GetName();
					bool is_selected = (strcmp(hoverMaterial, n) == 0);
				}
				if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
					hoverMaterial = mat[i]->GetName();
					SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::UI), 1);

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}

				}

			}
			ImGui::EndCombo();
		}
		if (hoverImg != nullptr) {
			ImGui::Image((void*)(hoverImg->gpuID), ImVec2(windowSize / 2, windowSize / 2), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size:\n Width: %dpx | Height: %dpx ", hoverImg->width, hoverImg->height);
			hoverMaterial = nullptr;
		}
	}

	if (ImGui::CollapsingHeader("Pressed Image")) {
		const char* pressedMaterial = NULL;
		if (pressedImg != nullptr) {
			pressedMaterial = pressedImg->GetName();
		}
		if (ImGui::BeginCombo("  ", pressedMaterial))//TO AVOID 
		{
			std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::UI);

			for (int i = 0; i < mat.size(); i++)
			{
				bool is_selected = false;
				if (pressedMaterial != nullptr) {
					const char* n = mat[i]->GetName();
					bool is_selected = (strcmp(pressedMaterial, n) == 0);
				}
				if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
					pressedMaterial = mat[i]->GetName();
					SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::UI), 2);

					if (is_selected) {
						ImGui::SetItemDefaultFocus();
					}

				}
			}
			ImGui::EndCombo();
		}
		if (pressedImg != nullptr) {
			float windowSize = ImGui::GetWindowContentRegionWidth();
			ImGui::Image((void*)(pressedImg->gpuID), ImVec2(windowSize / 2, windowSize / 2), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size:\n Width: %dpx | Height: %dpx ", pressedImg->width, pressedImg->height);
			pressedMaterial = nullptr;
		}

	}

	const char* fun = App->scene->functions->FunctionToString(function);
	if (ImGui::BeginCombo("OnClick", fun))//TO AVOID 
	{

		for (int i = 0; i < 4; i++)
		{
			bool is_selected = false;
			if (fun != nullptr) {
				const char* n = App->scene->functions->FunctionToString(i);
				bool is_selected = (strcmp(fun, n) == 0);
			}
			if (ImGui::Selectable(App->scene->functions->FunctionToString(i), is_selected)) {
				fun = App->scene->functions->FunctionToString(i);
				function = (Functions)i;
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}

			}
		}
		ImGui::EndCombo();
	}
}