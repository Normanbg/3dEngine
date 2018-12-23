#include "ComponentInputUI.h"
#include "ComponentTextUI.h"
#include "Application.h"
#include "ComponentButtonUI.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "ModuleResources.h"

#include "mmgr/mmgr.h"

ComponentInputUI::ComponentInputUI()
{
	typeUI = UI_INPUT;
	type = UI;
}


ComponentInputUI::~ComponentInputUI()
{
}

void ComponentInputUI::UpdateRectTransform()
{
}

void ComponentInputUI::DrawInspector()
{
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
}

void ComponentInputUI::DrawUI()
{
	
}

bool ComponentInputUI::Start()
{
	cText = myGO->GetComponentTextUI();
	cButton = myGO->GetComponentButtonUI();
	rectTransform = myGO->GetComponentRectTransform();
	cText->SetText("Insert Text Here");
	rectTransform->SetWidth(cText->GetLabelWidth());
	rectTransform->SetHeight(cText->GetLabelHeight());
	cButton->SetResource(App->resources->FindByName(DEFAULT_IDLE_IN, Resource::ResType::UI), 0);
	cButton->SetResource(App->resources->FindByName(DEFAULT_IDLE_IN, Resource::ResType::UI), 1);
	cButton->SetResource(App->resources->FindByName(DEFAULT_PRESSED_IN, Resource::ResType::UI), 2);

		return true;
}

bool ComponentInputUI::Update()
{
	if (App->gui->isMouseOnGame() && cButton->state == PRESSED) {
		
		readInput = true;		
	}
	if (cButton->state == ButtonState::PRESSED && !cButton->IsMouseOver()) {// to fix
		readInput = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) {
		if (textInput.size() > 0) {
			textInput.erase(textInput.end() - 1);
			cText->SetText(textInput.c_str());
		}
	}
	
		
	
	
	return true;
}

void ComponentInputUI::CleanUp()
{
}

void ComponentInputUI::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	alpha = data->GetFloat("Alpha", 1.0f);
	textInput = data->GetString("Txt", "");
	cText->SetText(textInput.c_str());
}

void ComponentInputUI::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	data.AddFloat("Alpha", alpha);
	data.AddString("Txt", textInput.c_str());
}

void ComponentInputUI::ReceiveEvent(const Event & event)
{
	if (event.type == event.input && readInput) {
		if (textInput.size() <= MAX_CHARS) {
			textInput += event.string;
			cText->SetText(textInput.c_str());
		}
	}
	
}

