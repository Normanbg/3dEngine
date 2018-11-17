#include "UIPanelAssets.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "ModuleResources.h"
#include "GameObject.h"
#include "ModuleScene.h"
#include "ModuleInput.h"

#include "mmgr/mmgr.h"

UIPanelAssets::UIPanelAssets(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelAssets::~UIPanelAssets()
{
}

void UIPanelAssets::Draw()
{
	uint flags = 0;
	flags |= ImGuiTreeNodeFlags_Leaf;
	ImGui::Begin(name.c_str(), &active);
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	
	App->fileSys->GetFilesFromDir(currDir.c_str(), files, dirs, false, true);
	ImGui::Text("%s", currDir.c_str());
	
	ImVec2 PRegion = ImGui::GetContentRegionAvail();
	
	ImGui::SameLine(PRegion.x-22);
	if (ImGui::SmallButton("Back" )) {
		App->fileSys->ShiftPath(&currDir);
	}
	
	ImGui::Separator();
	for (int i = 0; i < dirs.size(); i++) {		
		std::string dirName = dirs[i];
		if (ImGui::TreeNodeEx(dirName.c_str(), flags)) {
			if (ImGui::IsItemClicked(0))  {//if dir is clicked
				currDir += dirName;
			}
			ImGui::TreePop();
		}
	}
 static uint nodeClicked = -1;
;
	for (int i = 0; i < files.size(); i++) {
		std::string fileName;
		
		App->fileSys->GetNameFromPath( files[i].c_str(), nullptr, nullptr, &fileName, nullptr);		
		if (ImGui::BeginMenu(fileName.c_str())) {
			//if file is clicked
			if (ImGui::MenuItem("Load File")) {
				App->resources->LoadFiles(files[i].c_str());
			}
			if (ImGui::MenuItem("Delete File")) { App->fileSys->RemoveFile(files[i].c_str()); }
						
			ImGui::EndMenu();
		}
	}
	ImGui::End();
}

void UIPanelAssets::DrawChilds(std::vector<GameObject*> childs){
	for (std::vector<GameObject*>::iterator goIterator = childs.begin(); goIterator != childs.end(); goIterator++)
	{
		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if ((*goIterator) == App->scene->gObjSelected)
			flags |= ImGuiTreeNodeFlags_Selected;
		if ((*goIterator)->childrens.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;

	}
}


