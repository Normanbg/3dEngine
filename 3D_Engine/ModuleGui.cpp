#include "Application.h"
#include "ModuleGui.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"
#include "MathGeoLib\Geometry\GeometryAll.h"
#include "MathGeoLib\Math\MathAll.h"
#include "UIPanel.h"
#include "UIPanelAbout.h"
#include "UIPanelConfig.h"
#include "UIPanelConsole.h"
#include "UIPanelProperties.h"

#include "ModulePhysics3D.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
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

	uiPanels.push_back(panelAbout = new UIPanelAbout("About", 150, 150, 350, 350));
	uiPanels.push_back(panelConfig = new UIPanelConfig("Configuration", 1025, 0, 250, 550, true));
	uiPanels.push_back(panelConsole = new UIPanelConsole("Console", 50, 650, 800, 350, true));
	uiPanels.push_back(panelProperties = new UIPanelProperties("Properties", 775, 0, 250, 550, true));

	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();
	return true;
}

update_status ModuleGui::PreUpdate(float dt)
{
	BROFILER_CATEGORY("GUI_PreUpdate", Profiler::Color::Chartreuse);
	/*ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;*
	/*io.DisplaySize.y = 1280.0f;*/
	/*ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();*/
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleGui::Update(float dt)
{
	BROFILER_CATEGORY("GUI_Update", Profiler::Color::Chartreuse);
	if (ImGui::BeginMainMenuBar()) {
		//--------------HELP MENU
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Quit", "ESC"))
				return UPDATE_STOP;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Gui Demo"))
				demoShowcase = !demoShowcase;
			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/Normanbg/3dEngine/wiki");
			if (ImGui::MenuItem("Download latest"))
				App->RequestBrowser("https://github.com/Normanbg/3dEngine/releases");
			if (ImGui::MenuItem("Report a bug"))
				App->RequestBrowser("https://github.com/Normanbg/3dEngine/issues");
			if (ImGui::MenuItem("About")) {
				panelAbout->ChangeActive();
			}
			ImGui::EndMenu();
			//HELP MENU--------------
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Console"))
				panelConsole->ChangeActive();
			if (ImGui::MenuItem("Configuration"))
				panelConfig->ChangeActive();

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


update_status ModuleGui::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
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

void ModuleGui::AddConsoleLogs(const char* log){
	logsBuffer.push_back(log);
}
