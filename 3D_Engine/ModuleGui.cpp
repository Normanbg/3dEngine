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
		if (ImGui::CollapsingHeader("Create")) {
			if (ImGui::MenuItem("Sphere")) {
				math::Sphere s(vec(0, 0, 0), 2);
				App->physics->AddBody(s);
			}
			if (ImGui::MenuItem("Cyllinder")) {
				
			}
			if (ImGui::MenuItem("Capsule")) {
				math::Capsule ca(vec(0,0,0),vec(0,1,0), 5);
				App->physics->AddBody(ca);
			}
			if (ImGui::MenuItem("AABB")) {
				math::AABB ab(vec(0,0,0), vec(1,1,1));
				App->physics->AddBody(ab);
			}
			if (ImGui::MenuItem("OBB")) {
				//CREATE
			}
			if (ImGui::MenuItem("Frustum")) {
				//CREATE
			}
			if (ImGui::MenuItem("Planes")) {
				math::Plane p(vec(0,0,0),1);
				App->physics->AddBody(p);
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
		}	
		if (ImGui::CollapsingHeader("Random Generator")) {
			static bool a = false;
			if (ImGui::Button("Create Random Number")) {
				a ^= 1;
			}
			if (a)	{
				ImGui::Text("RANDOM NUMBER NEEDED TO CREATE");
			}
		}
		if (ImGui::MenuItem("CLOSE")) {
			return UPDATE_STOP;
			ImGui::EndMenu();
		}
	ImGui::ShowTestWindow();
	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate(float dt)
{
	App->physics->isIntersecting();
	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleGui::CleanUp()
{
	ImGui_ImplSdl_Shutdown();
	return true;
}
