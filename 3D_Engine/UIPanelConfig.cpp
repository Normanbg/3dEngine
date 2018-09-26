#include "UIPanelConfig.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

UIPanelConfig::UIPanelConfig(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name, positionX, positionY, width, height, active)
{
}

UIPanelConfig::~UIPanelConfig()
{
}

void UIPanelConfig::Draw() {

	(ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_MenuBar));

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Save")) { App->SaveGame(); }
			if (ImGui::MenuItem("Load")) { App->LoadGame(); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader("Application")) {

		const int maxSize = 64;

		std::string aux = App->window->GetWindowTitle();
		ImGui::InputText("App Name", (char*)aux.c_str(), maxSize);
		std::string aux2 = App->GetOrganization();
		//char str2[maxSize] = aux;
		ImGui::InputText("Organization", (char*)aux2.c_str(), maxSize);

		int DefaultFpsCap = (App->GetFramerateCap());

		ImGui::SliderInt("Max FPS", &(DefaultFpsCap), 1, 120);
		App->SetFramerateCap(DefaultFpsCap);
		ImGui::Text("Limit Framerate: %i", DefaultFpsCap);

		char title[20];
		std::vector<float> VecLog = App->GetFpsLog();
		sprintf_s(title, 20, "Framerate %.1f", VecLog[VecLog.size() - 1]);
		ImGui::PlotHistogram("##framerate", &VecLog[0], VecLog.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		VecLog = App->GetMsLog();
		sprintf_s(title, 20, "Milliseconds %.1f", VecLog[VecLog.size() - 1]);
		ImGui::PlotHistogram("##framerate", &VecLog[0], VecLog.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}

	if (ImGui::CollapsingHeader("Window")) {

		bool fScrB = App->window->IsFullscreen();
		if (ImGui::Checkbox("Fullscreen", &fScrB)) {
			fScrB != fScrB;
			App->window->SetFullscreen(fScrB);
		}

		ImGui::SameLine();
		bool borderB = App->window->IsBorderless();
		if (ImGui::Checkbox("Borderless", &borderB)) {
			borderB != borderB;
			App->window->SetBorderless(borderB);
		}

		bool reszB = App->window->IsResizable();
		if (ImGui::Checkbox("Resizable ", &reszB)) {
			reszB != reszB;
			App->window->SetResizable(reszB);
		}

		ImGui::SameLine();
		bool fullB = App->window->IsFullscreenDesktop();
		if (ImGui::Checkbox("Full Desktop", &fullB)) {
			fullB != fullB;
			App->window->SetFullscreenDesktop(fullB);
		}
		float bright = App->window->GetBrightness();
		ImGui::SliderFloat("Bright", &bright, 0, 1.0f);
		App->window->SetBrightness(bright);

		int w, h;
		App->window->GetSize(w, h);
		int minWidthVal = 640;
		int maxWidthVal = 1920;
		if (ImGui::SliderInt("Width", &w, minWidthVal, maxWidthVal)) {
			App->window->SetSize(w, h);
		}
		int minHeightVal = 480;
		int maxHeightVal = 1080;
		if (ImGui::SliderInt("Height", &h, minHeightVal, maxHeightVal)) {
			App->window->SetSize(w, h);
		}
	}

	App->input->InputData();
	App->HardwareData();
	
	ImGui::End();

}
