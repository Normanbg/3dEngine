#include "UIPanelSceneInfo.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "ModuleGui.h"
#include "ModuleTime.h"

#include "mmgr/mmgr.h"

UIPanelSceneInfo::UIPanelSceneInfo(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
	selectedFile[0] = '\0';
}

UIPanelSceneInfo::~UIPanelSceneInfo()
{
}

void UIPanelSceneInfo::Draw()
{
	uint flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	ImGui::Begin("Scene Info", &active, flags);

	if (wantToLoadFile && FileState(OWN_FILE_FORMAT)) {

		const char* fileName = CloseFileState();
		if (fileName != nullptr)
			App->importer->LoadMeshPEI(fileName);
			wantToLoadFile = false;

	}
	if (fileState == opened) {
		LoadFilePopUp((fileStateExtensionFilter.length() > 0) ? fileStateExtensionFilter.c_str() : nullptr);
	}
	else
		inModal = false;

	ImVec2 PRegion = ImGui::GetContentRegionAvail();
	ImGui::SetCursorPosX(PRegion.x / 2 - 40);
	if (ImGui::Button("Play", { 40, 20 }))
	{
		if (App->time->IsPaused()) {
			App->time->Resume();
		}
		else {
			if (!App->time->IsPlaying()) {
				App->time->Play();
				App->scene->SaveScene(TEMP_SCENE_FILE);
			}
		}
		App->scene->inGame = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause", { 40, 20 }))
	{
		if (App->scene->inGame && !App->time->IsPaused()) {
			App->time->Pause();
		}

	}
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f,0,0,1 });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0.2f,0,1 });
	if (ImGui::Button("Stop", { 40, 20 }))
	{
		if (App->scene->inGame) {
			App->time->Stop();
			App->scene->inGame = false;
			App->scene->LoadScene(TEMP_SCENE_FILE);
		}

	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


	ImGui::SliderFloat("Time Scale", App->time->GetTimeScale(), 0, 3.0f, "%.1f");
	if (ImGui::SmallButton("Reset")) { App->time->SetTimeScale(1.0f); }
	ImGui::Text("");
	ImGui::Text("Real Time: %.1f", App->time->GetRealTimeSec());
	ImGui::Text("Game Time: %.1f", App->time->GetGameTimeSec());
	
	ImGui::Text("");
	ImGui::Text("Game dt: %.3f", App->time->GetGameDeltaTime());

	ImGui::End();
}


void UIPanelSceneInfo::DrawDirectoryRecursive(const char * directory, const char * filter_extension)
{
	std::vector<std::string> files;
	std::vector<std::string> directories;

	std::string dir((directory) ? directory : "");
	dir += "/";

	App->fileSys->GetFilesFromDir(dir.c_str(), files, directories);

	for (int i = 0; i < directories.size(); i++)
	{
		if (ImGui::TreeNodeEx((dir + directories[i]).c_str(), 0, "%s/", directories[i].c_str()))
		{
			DrawDirectoryRecursive((dir + directories[i]).c_str(), filter_extension);
			ImGui::TreePop();
		}
	}

	std::sort(files.begin(), files.end());

	for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
	{
		const std::string& str = *it;

		bool estensionMatch = true;

		if (filter_extension && str.substr(str.find_last_of(".")) != filter_extension)
			estensionMatch = false;

		if (estensionMatch && ImGui::TreeNodeEx(str.c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked()) {
				sprintf_s(selectedFile, FILE_MAX, "%s%s", dir.c_str(), str.c_str());

				if (ImGui::IsMouseDoubleClicked(0))
					fileState = toClose;
			}

			ImGui::TreePop();
		}
	}

}

bool UIPanelSceneInfo::FileState(const char * extension, const char* rootFolder)
{
	bool ret = true;

	switch (fileState)
	{
	case closed:
		selectedFile[0] = '\0';
		fileStateExtensionFilter = (extension) ? extension : "";
		fileStateOrigin = (rootFolder) ? rootFolder : "";
		fileState = opened;
	case opened:
		ret = false;
		break;
	}

	return ret;
}

const char * UIPanelSceneInfo::CloseFileState()
{
	if (fileState == toClose)
	{
		fileState = closed;
		return selectedFile[0] ? selectedFile : nullptr;
	}
	return nullptr;
}

void UIPanelSceneInfo::LoadFilePopUp(const char* extensionFilter, const char* rootDirectory)
{
	ImGui::OpenPopup("Load File");

	if (ImGui::BeginPopupModal("Load File", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		inModal = true;

		//ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::BeginChild("File Browser", ImVec2(0, 300), true);
		DrawDirectoryRecursive(rootDirectory, extensionFilter);
		ImGui::EndChild();
		//ImGui::PopStyleVar();

		ImGui::PushItemWidth(250.f);
		if (ImGui::InputText("##file_selector", selectedFile, FILE_MAX, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			fileState = toClose;

		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGui::Button("Ok", ImVec2(50, 20)))
			fileState = toClose;
		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(50, 20)))
		{
			fileState = toClose;
			selectedFile[0] = '\0';
		}


	}
	else
		inModal = false;

	ImGui::EndPopup();
}
