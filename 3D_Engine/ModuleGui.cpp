#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "MathGeoLib/Geometry/GeometryAll.h"
#include "UIPanel.h"
#include "UIPanelAbout.h"
#include "UIPanelConfig.h"
#include "UIPanelConsole.h"
#include "UIPanelInspector.h"
#include "UIPanelHierarchy.h"
#include "UIPanelMaterials.h"
#include "UIPanelScene.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleEditorCamera.h"
#include "Brofiler/Brofiler.h"

#include <list>


ModuleGui::ModuleGui(bool start_enabled) : Module(start_enabled)
{
	name = "Gui";
}


ModuleGui::~ModuleGui()
{
	for (std::list < UIPanel* > ::reverse_iterator item = uiPanels.rbegin(); item != uiPanels.rend(); item++) {
		delete (*item);
	}
	uiPanels.clear();
}

bool ModuleGui::Start()
{
	BROFILER_CATEGORY("GUI_Start", Profiler::Color::Chartreuse);
	
	dropFX = App->audio->LoadFx("fx/drop.wav");
	closeFX = App->audio->LoadFx("fx/close.wav");
	openFX = App->audio->LoadFx("fx/open.wav");
	searchingFX = App->audio->LoadFx("fx/searching.wav");

	uiPanels.push_back(panelAbout = new UIPanelAbout("About", 150, 150, 350, 350));
	uiPanels.push_back(panelConfig = new UIPanelConfig("Configuration", 1025, 15, 250, 550, true));
	uiPanels.push_back(panelConsole = new UIPanelConsole("Console", 50, 650, 1165, 350, true));
	uiPanels.push_back(panelInspector = new UIPanelInspector("Inspector", 775, 15, 250, 550, true));
	uiPanels.push_back(panelHierarchy = new UIPanelHierarchy("Hierarchy", 0, 15, 250, 550, true));
	uiPanels.push_back(panelMaterial = new UIPanelMaterials("Materials", 0, 399, 240, 406, true));
	uiPanels.push_back(panelScene = new UIPanelScene("Scene", 0, 399, 240, 406, true));

	ImGui::CreateContext();
	demoShowcase = false;
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

	io = &ImGui::GetIO();
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
	
	return true;
}

update_status ModuleGui::PreUpdate(float dt)
{
	BROFILER_CATEGORY("GUI_PreUpdate", Profiler::Color::Chartreuse);
	
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleGui::Update(float dt)
{
	BROFILER_CATEGORY("GUI_Update", Profiler::Color::Chartreuse);
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Clear scene"))
				App->scene->ClearScene();
			if (ImGui::MenuItem("Save scene", "4"))
				App->scene->SaveScene();
			if (ImGui::MenuItem("Load scene", "5"))
				App->scene->LoadScene();			
			if (ImGui::MenuItem("Quit", "ESC"))
				return UPDATE_STOP;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Gui Demo")){
				if (demoShowcase) {
					App->audio->PlayFx(closeFX);
				}
				else
					App->audio->PlayFx(openFX);
				demoShowcase = !demoShowcase;
				}
			if (ImGui::MenuItem("Documentation")) {
				App->RequestBrowser("https://github.com/Normanbg/3dEngine/wiki");
				App->audio->PlayFx(searchingFX);
			}
			if (ImGui::MenuItem("Download latest")) {
				App->RequestBrowser("https://github.com/Normanbg/3dEngine/releases");
				App->audio->PlayFx(searchingFX);
			}
			if (ImGui::MenuItem("Report a bug")) {
				App->RequestBrowser("https://github.com/Normanbg/3dEngine/issues");
				App->audio->PlayFx(searchingFX);
			}
			if (ImGui::MenuItem("About", NULL, panelAbout->isEnabled())) {
				if (panelAbout->isEnabled()) {
					App->audio->PlayFx(closeFX);
				}
				else
					App->audio->PlayFx(openFX);
				panelAbout->ChangeActive();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Console", NULL, panelConsole->isEnabled())) {
				if (panelConsole->isEnabled()) {
					App->audio->PlayFx(closeFX);
				}
				else
					App->audio->PlayFx(openFX);
				panelConsole->ChangeActive();
			}
			if (ImGui::MenuItem("Configuration", NULL, panelConfig->isEnabled())) {
				if (panelConfig->isEnabled()) {
					App->audio->PlayFx(closeFX);
				}
				else
					App->audio->PlayFx(openFX);
				panelConfig->ChangeActive();
			}
			if (ImGui::MenuItem("Inspector", NULL, panelInspector->isEnabled())) {
				if (panelInspector->isEnabled()) {
					App->audio->PlayFx(closeFX);
				}
				else
					App->audio->PlayFx(openFX);
				panelInspector->ChangeActive();
			}
			if (ImGui::MenuItem("Hierarchy", NULL, panelHierarchy->isEnabled())) {
				if (panelHierarchy->isEnabled()) {
					App->audio->PlayFx(closeFX);
				}
				else
					App->audio->PlayFx(openFX);
				panelHierarchy->ChangeActive();
			}
			if (ImGui::MenuItem("Materials", NULL, panelMaterial->isEnabled())) {
				if (panelMaterial->isEnabled()) {
					App->audio->PlayFx(closeFX);
				}
				else
					App->audio->PlayFx(openFX);
				panelMaterial->ChangeActive();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	
	return UPDATE_CONTINUE;
}

void ModuleGui::Draw() {

	BROFILER_CATEGORY("GUI_Draw", Profiler::Color::Chartreuse);
	SetWinDockInv();
	if (demoShowcase)
		ImGui::ShowTestWindow();

	for (std::list<UIPanel*>::iterator iterator = uiPanels.begin(); iterator != uiPanels.end(); iterator++) {
		if ((*iterator)->isEnabled()) {
			ImGui::SetNextWindowPos({ (*iterator)->positionX, (*iterator)->positionY }, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize({ (*iterator)->width, (*iterator)->height }, ImGuiCond_FirstUseEver);
			(*iterator)->Draw();
		}
	}
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

}



bool ModuleGui::CleanUp()
{
	BROFILER_CATEGORY("GUI_CleanUp", Profiler::Color::Chartreuse);
	for (std::list<UIPanel*>::iterator iterator = uiPanels.begin(); iterator != uiPanels.end(); iterator++)
	{
		if (*iterator != nullptr)
			delete *iterator;
		*iterator = nullptr;
	}

	uiPanels.clear();

	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL2_Shutdown();
	return true;
}

void ModuleGui::AddConsoleLogs(const char* log)  {
	logsBuffer.push_back(log);
}


void ModuleGui::ImplGuiInputs(SDL_Event * e) const{
	ImGui_ImplSDL2_ProcessEvent(e);
}

bool ModuleGui::isMouseOnScene() const {
	return mouseOnScene;
}

void ModuleGui::MouseOnScene(bool mouseScene)
{
	mouseOnScene = mouseScene;
}

void ModuleGui::SetWinDockInv(){	
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruDockspace;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	ImGui::End();
}
