#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "MathGeoLib\Geometry\GeometryAll.h"
#include "MathGeoLib\Math\MathAll.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"





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
	if (ImGui::CollapsingHeader("MENU"))
	{

		if (ImGui::BeginMenu("Create")) {
			if (ImGui::MenuItem("Sphere")) {
				//CREATE				
			}
			if (ImGui::MenuItem("Cyllinder")) {
				//CREATE

			}
			if (ImGui::MenuItem("Capsule")) {
				//CREATE

			}
			if (ImGui::MenuItem("AABB")) {
				//CREATE

			}
			if (ImGui::MenuItem("OBB")) {
				//CREATE

			}
			if (ImGui::MenuItem("Frustum")) {
				//CREATE

			}
			if (ImGui::MenuItem("Planes")) {
				//CREATE

			}
			if (ImGui::MenuItem("Segments")) {
				//CREATE

			}
			if (ImGui::MenuItem("Rays")) {
				//CREATE

			}
			if (ImGui::MenuItem("Convex Hull")) {
				//CREATE

			}
			if (ImGui::MenuItem("Mesh")) {
				//CREATE
			}
			if (ImGui::MenuItem("Triangles")) {
				//CREATE

			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("CLOSE")) {
			return UPDATE_STOP;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Random Number")) {
			if (ImGui::Button("Generate")) {
			

			}

			ImGui::EndMenu();
		}

		//ImGui::EndMenu();
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
