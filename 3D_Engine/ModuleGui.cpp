#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"
#include "MathGeoLib\Geometry\GeometryAll.h"
#include "UIPanel.h"
#include "UIPanelAbout.h"
#include "UIPanelConfig.h"
#include "UIPanelConsole.h"
#include "UIPanelProperties.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
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
	demoShowcase = false;
	dropFX = App->audio->LoadFx("fx/drop.wav");
	closeFX = App->audio->LoadFx("fx/close.wav");
	openFX = App->audio->LoadFx("fx/open.wav");
	searchingFX = App->audio->LoadFx("fx/searching.wav");

	uiPanels.push_back(panelAbout = new UIPanelAbout("About", 150, 150, 350, 350));
	uiPanels.push_back(panelConfig = new UIPanelConfig("Configuration", 1025, 15, 250, 550, true));
	uiPanels.push_back(panelConsole = new UIPanelConsole("Console", 50, 650, 1165, 350, true));
	uiPanels.push_back(panelProperties = new UIPanelProperties("Properties", 775, 15, 250, 550, true));

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();

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
			if (ImGui::MenuItem("Properties", NULL, panelProperties->isEnabled())) {
				if (panelProperties->isEnabled()) {
					App->audio->PlayFx(closeFX);
				}
				else
					App->audio->PlayFx(openFX);
				panelProperties->ChangeActive();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

	}
	
	return UPDATE_CONTINUE;
}

void ModuleGui::Draw() {

	BROFILER_CATEGORY("GUI_Draw", Profiler::Color::Chartreuse);
	if (demoShowcase)
		ImGui::ShowTestWindow();

	for (std::list<UIPanel*>::iterator iterator = uiPanels.begin(); iterator != uiPanels.end(); iterator++) {
		if ((*iterator)->isEnabled()) {
			ImGui::SetNextWindowPos({ (*iterator)->positionX, (*iterator)->positionY });
			ImGui::SetNextWindowSize({ (*iterator)->width, (*iterator)->height }, ImGuiSetCond_Once);
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

bool ModuleGui::MouseOnGui() const {

	return ImGui::GetIO().WantCaptureMouse;
}
