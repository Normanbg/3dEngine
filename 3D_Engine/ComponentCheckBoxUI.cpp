#include "ComponentCheckBoxUI.h"
#include "ComponentImageUI.h"

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
	//image = myGO->GetComponentImageUI();
	//rectTransform = myGO->GetComponentRectTransform();
	return true;
}

bool ComponentCheckBoxUI::Update()
{
	return true;
}

void ComponentCheckBoxUI::CleanUp()
{
}

void ComponentCheckBoxUI::Load(Config * data)
{
}

void ComponentCheckBoxUI::Save(Config & data) const
{
}

void ComponentCheckBoxUI::ChangeGOImage()
{
}

const bool ComponentCheckBoxUI::HasTexture(ResourceTexture * res) const
{
	return false;
}

void ComponentCheckBoxUI::SetResource(uuid resource, int numRes)
{
}
