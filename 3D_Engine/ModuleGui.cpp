#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"


ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleGui::~ModuleGui()
{
}

bool ModuleGui::Start()
{

	ImGui_ImplSdl_Init(App->window->window);
	return true;
}

update_status ModuleGui::PreUpdate(float dt)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;
	ImGui_ImplSdl_NewFrame(App->window->window);
	return UPDATE_CONTINUE;
}

update_status ModuleGui::Update(float dt)
{
	if (ImGui::BeginMenu("CLOSE"))
	{
		return UPDATE_STOP;
		ImGui::EndMenu();
	}
	ImGui::ShowTestWindow();
	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate(float dt)
{
	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleGui::CleanUp()
{
	ImGui_ImplSdl_Shutdown();
	return true;
}
