#include "ComponentInputUI.h"
#include "ComponentTextUI.h"
#include "Application.h"
#include "ComponentButtonUI.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "ModuleGui.h"

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

		return true;
}

bool ComponentInputUI::Update()
{
	if (App->gui->isMouseOnGame() && cButton->state == PRESSED) {
		
		readInput = true;		
	}
	if (readInput && cButton->state == IDLE && App->input->GetMouseButton(0)) {
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
}

void ComponentInputUI::Save(Config & data) const
{
}

void ComponentInputUI::ReceiveEvent(const Event & event)
{
	if (event.type == event.input && readInput) {

		textInput += event.string;
		cText->SetText(textInput.c_str());
	}
	
}

