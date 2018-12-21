#include "ComponentInputUI.h"
#include "ComponentTextUI.h"
#include "ComponentButtonUI.h"
#include "GameObject.h"

ComponentInputUI::ComponentInputUI()
{
}


ComponentInputUI::~ComponentInputUI()
{
}

void ComponentInputUI::UpdateRectTransform()
{
}

void ComponentInputUI::DrawInspector()
{
}

void ComponentInputUI::DrawUI()
{
}

bool ComponentInputUI::Start()
{
	cText = myGO->GetComponentTextUI();
	cButton = myGO->GetComponentButtonUI();
		
	return true;
}

bool ComponentInputUI::Update()
{
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
