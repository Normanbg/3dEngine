#include "UIPanelConfig.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"

#include "mmgr/mmgr.h"

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

		static const int maxSize = 64;

		std::string aux = App->window->GetWindowTitle();
		if (ImGui::InputText("App Name", (char*)aux.c_str(), maxSize)) {
			App->window->SetTitle((char*)aux.c_str());
		}
		aux = App->GetOrganization();
		//char str2[maxSize] = aux;
		if (ImGui::InputText("Organization", (char*)aux.c_str(), maxSize)) {
			App->SetOrganization((char*)aux.c_str());
		}
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
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse Position:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d, %d", App->input->GetMouseX(), App->input->GetMouseY());
		ImGui::Text("Mouse Motion:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d, %d", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

		ImGui::Text("Mouse Wheel:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", App->input->GetMouseZ());
	}
	if (ImGui::CollapsingHeader("Hardware"))
	{
		float totalVidMem = 0;
		float currVidMem = 0;
		float availVidMem = 0;
		float reserVidMem = 0;		

		SDL_version version;
		SDL_GetVersion(&version);

		ImVec4 yellow(1, 1, 0, 1);

		ImGui::Text("SDL Version"); ImGui::SameLine();
		ImGui::TextColored(yellow, "%d.%d.%d", version.major, version.minor, version.patch);
		ImGui::Separator();
		ImGui::Text("CPU's:"); ImGui::SameLine();
		ImGui::TextColored(yellow, "%d", SDL_GetCPUCount()); ImGui::SameLine();
		ImGui::TextColored(yellow, "(Cache: %dkb)", SDL_GetCPUCacheLineSize());

		ImGui::Text("System RAM:"); ImGui::SameLine();
		ImGui::TextColored(yellow, "%0.1fGb", ((float)SDL_GetSystemRAM() / 1024));

		ImGui::Text("Caps: "); ImGui::SameLine();

		if (SDL_Has3DNow()) ImGui::TextColored(yellow, "3DNow,"); ImGui::SameLine();
		if (SDL_HasAVX()) ImGui::TextColored(yellow, "AVX,"); ImGui::SameLine();
		if (SDL_HasAltiVec()) ImGui::TextColored(yellow, "AltiVec,"); ImGui::SameLine();
		if (SDL_HasMMX()) ImGui::TextColored(yellow, "MMX,"); ImGui::SameLine();
		if (SDL_HasRDTSC()) ImGui::TextColored(yellow, "RDTSC,"); ImGui::SameLine();
		if (SDL_HasSSE()) ImGui::TextColored(yellow, "SSE,"); ImGui::SameLine();
		if (SDL_HasSSE2()) ImGui::TextColored(yellow, "SSE2,"); ImGui::SameLine();
		if (SDL_HasSSE3()) ImGui::TextColored(yellow, "SSE3,"); ImGui::SameLine();
		if (SDL_HasSSE41()) ImGui::TextColored(yellow, "SSE41,"); ImGui::SameLine();
		if (SDL_HasSSE42()) ImGui::TextColored(yellow, "SSE42,");

		ImGui::Separator();

		ImGui::Text("GPU: "); ImGui::SameLine();
		ImGui::TextColored(yellow, " vendor %s", App->renderer3D->GetGraphicsVendor());

		ImGui::Text("Brand: "); ImGui::SameLine();
		ImGui::TextColored(yellow, "%s", App->renderer3D->GetGraphicsModel());

		ImGui::Text("VRAM Budget: "); ImGui::SameLine();
		ImGui::TextColored(yellow, "%.2f Mb", App->GetTotalVideoMem());

		ImGui::Text("VRAM Usage: "); ImGui::SameLine();
		ImGui::TextColored(yellow, "%.2f Mb", App->GetCurrentVideoMem());

		ImGui::Text("VRAM Avaliable: "); ImGui::SameLine();
		ImGui::TextColored(yellow, "%.2f Mb", App->GetAvaliableVideoMem());

		ImGui::Text("VRAM Reserved: "); ImGui::SameLine();
		ImGui::TextColored(yellow, "%.2f Mb", App->GetReservedVideoMem());
		
		updates++;

		sMStats stats = m_getMemoryStatistics();

		if (updates > MEMORY_UPDATE_READING)
		{			
			updates = 0;
			if (memory.size() > MAX_FPS_LOG) {			
				memory.erase(memory.begin());				
			}
			memory.push_back(stats.totalReportedMemory);
		}

		char title[20];
		sprintf_s(title, 20, "Memory Consumption");
		ImGui::PlotHistogram("##memory", &memory[0], memory.size(), 0, title, 0.0f, stats.peakReportedMemory * 1.5f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Render")) {
		/*bool depthTest = App->renderer3D->GetDepthTest();
		if (ImGui::Checkbox("Depth Test ", &depthTest)) {			
			App->renderer3D->SetDepthTest(depthTest);
		}*/
		ImGui::SameLine();
		bool cullFace = App->renderer3D->GetCullFace();
		if (ImGui::Checkbox("Cull Face ", &cullFace)) {
			App->renderer3D->SetCullFace(cullFace);
		}
		/*bool light = App->renderer3D->GetLighting();
		if (ImGui::Checkbox("Lightning ", &light)) {
			App->renderer3D->SetLighting(light);
		}*/
		ImGui::SameLine();
		bool colormat = App->renderer3D->GetColorMaterial();
		if (ImGui::Checkbox("Color Material", &colormat)) {
			App->renderer3D->SetColorMaterial(colormat);
		}
		bool tex2D = App->renderer3D->GetTexture2D();
		if (ImGui::Checkbox("Texture2D", &tex2D)) {
			App->renderer3D->SetTexture2D(tex2D);
		}		
		bool grid = App->renderer3D->GetGrid();
		if (ImGui::Checkbox("Grid ", &grid)) {
			App->renderer3D->SetGrid(grid);
		}
		ImGui::SameLine();
		bool axis = App->renderer3D->GetAxis();
		if (ImGui::Checkbox("Axis", &axis)) {
			App->renderer3D->SetAxis(axis);
		}
		bool wiref = App->renderer3D->GetWireframe();
		if (ImGui::Checkbox("Wireframe", &wiref)) {
			App->renderer3D->SetWireframe(wiref);
		}
		ImGui::SameLine();
		bool normals = App->renderer3D->GetNormals();
		if (ImGui::Checkbox("FBX Normals", &normals)) {
			App->renderer3D->SetNormals(normals);
		}
		bool bBox = App->renderer3D->GetBoundingBox();
		if (ImGui::Checkbox("Bounding Box", &bBox)) {
			App->renderer3D->SetBoundingBox(bBox);
		}
		ImGui::SameLine();
		bool quadtree = App->renderer3D->GetQuadTree();
		if (ImGui::Checkbox("Quadtree", &quadtree)) {
			App->renderer3D->SetQuadTree(quadtree);
		}
		bool ray = App->renderer3D->GetRay();
		if (ImGui::Checkbox("Mouse Ray", &ray)) {
			App->renderer3D->SetRay(ray);
		}
	}
	
	if (ImGui::CollapsingHeader("Camera")) {
		App->camera->ConfigInfo();
		
	}
	ImGui::End();

}
