#include "ComponentCheckBoxUI.h"
#include "ComponentImageUI.h"
#include "GameObject.h"
#include "ModuleResources.h"
#include "ModuleGui.h"
#include "UIPanelGame.h"
#include "ModuleInput.h"

#include "mmgr/mmgr.h"

ComponentCheckBoxUI::ComponentCheckBoxUI()
{
	typeUI = UI_CHECKBOX;
	type = UI;
}


ComponentCheckBoxUI::~ComponentCheckBoxUI()
{
}

void ComponentCheckBoxUI::DrawInspector()
{
}

bool ComponentCheckBoxUI::Start()
{
	image = myGO->GetComponentImageUI();
	rectTransform = myGO->GetComponentRectTransform();
	rectTransform->SetWidth(20);
	rectTransform->SetHeight(20);
	return true;
}

bool ComponentCheckBoxUI::Update()
{
	return true;
}

void ComponentCheckBoxUI::CleanUp()
{
	if (HasTexture(unpressedImg)) {
		unpressedImg->CleanUp();
	}
	if (HasTexture(pressedImg)) {
		pressedImg->CleanUp();
	}

	rectTransform = nullptr;
	unpressedImg = nullptr;
	pressedImg = nullptr;
	myGO = nullptr;
	image = nullptr;
}

void ComponentCheckBoxUI::CheckState() {
	if (IsMouseOver()) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state == PRESSED) {
			state = IDLE;
			if (unpressedImg != nullptr)
				ChangeGOImage();
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && state != PRESSED) {
			state = PRESSED;
			if (pressedImg != nullptr)
				ChangeGOImage();
		}
	}
}


bool ComponentCheckBoxUI::IsMouseOver()
{
	float2 mousePos = float2(App->gui->panelGame->GetMouseRelativeToGame().x, App->gui->panelGame->GetMouseRelativeToGame().y);
	if (rectTransform->GetGlobalPos().x <= mousePos.x && mousePos.x <= rectTransform->GetGlobalPos().x + rectTransform->GetWidth() &&
		rectTransform->GetGlobalPos().y <= mousePos.y && mousePos.y <= rectTransform->GetGlobalPos().y + rectTransform->GetHeight()) {
		return true;
	}
	else
		return false;
}

void ComponentCheckBoxUI::ChangeGOImage()
{
	switch (state)
	{
	case ButtonState::IDLE:
		image->SetResource(App->resources->FindByName(unpressedImg->GetName(), Resource::ResType::Texture));
		break;
	case ButtonState::PRESSED:
		image->SetResource(App->resources->FindByName(pressedImg->GetName(), Resource::ResType::Texture));
		break;
	default:
		break;
	}
}

const bool ComponentCheckBoxUI::HasTexture(ResourceTexture * res) const
{
	bool ret;
	res ? ret = true : ret = false;
	return ret;
}

void ComponentCheckBoxUI::SetResource(uuid resource, int numRes)
{
	if (resource != 0) {
		switch (numRes)
		{
		case 0:
			unpressedImg = (ResourceTexture*)App->resources->Get(resource);
			unpressedImg->LoadInMemory();
			ChangeGOImage();
			break;
		case 1:
			pressedImg = (ResourceTexture*)App->resources->Get(resource);
			pressedImg->LoadInMemory();
			break;
		}
	}
}

void ComponentCheckBoxUI::Load(Config * data)
{
}

void ComponentCheckBoxUI::Save(Config & data) const
{
}
