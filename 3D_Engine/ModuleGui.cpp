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
	demoShowcase = false;
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
	
	if (ImGui::BeginMainMenuBar()) {
		//--------------HELP MENU
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Gui Demo"))
				demoShowcase = !demoShowcase;
			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/Normanbg/3dEngine/wiki");
			if (ImGui::MenuItem("Download latest"))
			App->RequestBrowser("https://github.com/Normanbg/3dEngine/releases");
			if (ImGui::MenuItem("Report a bug"))
			App->RequestBrowser("https://github.com/Normanbg/3dEngine/issues");
			if (ImGui::MenuItem("About")) {}
			//requestBrowser
			ImGui::EndMenu();
		//HELP MENU--------------
		}
		ImGui::EndMainMenuBar();
	}
			/*MAX & MIN
			ImGui::InputInt("Max Number", &var);
			ImGui::InputInt("Min Number", &var2);
			if (ImGui::SmallButton("Generate Random Int")) {
				randomNum = ("%i", (int)pcg16si_boundedrand_r(&rng, var) + var2);
			}*/		
	if (ImGui::CollapsingHeader("Create")) {
		if (ImGui::MenuItem("Sphere")) {
			math::Sphere s(vec(0, 0, 0), 2);
			App->physics->AddBody(s);
		}
		if (ImGui::MenuItem("Cyllinder")) {

		}
		if (ImGui::MenuItem("Capsule")) {
			math::Capsule ca(vec(0, 0, 0), vec(0, 1, 0), 5);
			App->physics->AddBody(ca);
		}
		if (ImGui::MenuItem("AABB")) {
			math::AABB ab(vec(0, 0, 0), vec(1, 1, 1));
			App->physics->AddBody(ab);
		}
		if (ImGui::MenuItem("OBB")) {
			//CREATE
		}
		if (ImGui::MenuItem("Frustum")) {
			//CREATE
		}
		if (ImGui::MenuItem("Planes")) {
			math::Plane p(vec(0, 0, 0), 1);
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
		if (ImGui::Button("Create Random Number Between 0.0 & 1.0")) {
			a ^= 1;
		}
		if (a) {
			for (int i = 1; i < 101; i++) {
				pcg32_random_t randNumbGen;
				double d = ldexp(pcg32_random_r(&randNumbGen), -32);
				ImGui::Text("Random Number %d: %f", i, d);

			}
		}
	}
	if (ImGui::MenuItem("CLOSE")) {
		return UPDATE_STOP;
		ImGui::EndMenu();
	}


	if (demoShowcase)
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
