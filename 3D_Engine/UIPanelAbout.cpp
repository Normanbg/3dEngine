#include "UIPanelAbout.h"
#include "Application.h"
#include "ImGui/imgui.h"

UIPanelAbout::UIPanelAbout(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name,positionX,positionY, width, height, active)
{
}

UIPanelAbout::~UIPanelAbout()
{
}

void UIPanelAbout::Draw(){
	ImGui::Begin(name.c_str(), &active);

	ImGui::Text("Video Game Engine. \n\nThis Engine is done for the 3D Engines subject on the Design & Development Degree by UPC \n\nDeveloped by:\n");
	if (ImGui::SmallButton("Norman Benet"))
		App->RequestBrowser("https://github.com/normanbg");
	if (ImGui::SmallButton("Carles Margeli"))
		App->RequestBrowser("https://github.com/Margeli");
	ImGui::Spacing();
	ImGui::Text("Libraries Used:");
	SDL_version version;
	SDL_GetVersion(&version);

	char sdlVersion[30];
	sprintf_s(sdlVersion, 30, "SDL (Version: %d.%d.%d)", version.major, version.minor, version.patch);
	if (ImGui::SmallButton(sdlVersion))
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
