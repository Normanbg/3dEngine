#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui\imgui_impl_opengl2.h"
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
	
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL2_Init();
	return true;
}

update_status ModuleGui::PreUpdate(float dt)
{
	/*ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;*/
	/*io.DisplaySize.y = 1280.0f;*/
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleGui::Update(float dt)
{

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
			if (ImGui::MenuItem("About"))
				activeAbout = !activeAbout;
			ImGui::EndMenu();
			//HELP MENU--------------
		}
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Console")) {}
			//open console
			if (ImGui::MenuItem("Configuration"))
				configActive = !configActive;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Hw")) {
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

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();

	}
	/*MAX & MIN
	ImGui::InputInt("Max Number", &var);
	ImGui::InputInt("Min Number", &var2);
	if (ImGui::SmallButton("Generate Random Int")) {
		randomNum = ("%i", (int)pcg16si_boundedrand_r(&rng, var) + var2);
	}*/

	if (configActive) {
		ImGui::SetNextWindowSize(ImVec2(650, 350), ImGuiSetCond_Once);
		if (ImGui::Begin("Configuration", &configActive)) {

			if (ImGui::CollapsingHeader("Application")) {

				const int maxSize = 64;
				char str[maxSize] = TITLE;
				ImGui::InputText("App Name", str, maxSize);

				char str2[maxSize] = ORGANIZATION;
				ImGui::InputText("Organization", str2, maxSize);

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
			/*if (ImGui::CollapsingHeader("Hardware")) {*/
				/*ImVec4 yellow(255, 255, 0, 255);
				ImGui::Text("CPUs:");
				ImGui::SameLine();
				ImGui::TextColored(yellow, "%d (Cache: %dKb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

				ImGui::Text("System RAM:");
				ImGui::SameLine();
				ImGui::TextColored(yellow, "%0.1fGb", ((float)SDL_GetSystemRAM() / 1024));
				}*/
			App->input->InputData();
			App->HardwareData();

			ImGui::End();
		}
	}

	if (demoShowcase)
		ImGui::ShowTestWindow();

	if (activeAbout) {
		if (ImGui::Begin("About")) {
			ImGui::Text("Video Game Engine. \n\nThis Engine is done for the 3D Engines subject on the Design & Development Degree by UPC \n\nDeveloped by:\n");
			if (ImGui::SmallButton("Norman Benet"))
				App->RequestBrowser("https://github.com/normanbg");
			if (ImGui::SmallButton("Carles Margeli"))
				App->RequestBrowser("https://github.com/Margeli");
			ImGui::Spacing();
			ImGui::Text("Libraries Used:");
			if (ImGui::SmallButton("SDL"))
				App->RequestBrowser("https://www.libsdl.org/download-2.0.php");
			if (ImGui::SmallButton("OpenGL")) 
				App->RequestBrowser("https://github.com/ocornut/imgui");
			if (ImGui::SmallButton("ImGui"))
				App->RequestBrowser("https://www.opengl.org/");
			if (ImGui::SmallButton("MathGeoLIB"))
				App->RequestBrowser("https://github.com/juj/MathGeoLib");
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Text("License:");
			if (ImGui::SmallButton("MIT"))
				App->RequestBrowser("https://github.com/Normanbg/3dEngine/blob/master/LICENSE");
			ImGui::End();
		}

	}


	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate(float dt)
{
	App->physics->isIntersecting();
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

bool ModuleGui::CleanUp()
{
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL2_Shutdown();
	return true;
}
