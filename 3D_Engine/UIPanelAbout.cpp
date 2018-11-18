#include "UIPanelAbout.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ModuleAudio.h"
#include "ModuleGui.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Assimp/include/version.h"

#include "mmgr/mmgr.h"

UIPanelAbout::UIPanelAbout(const char * name, float positionX, float positionY, float width, float height, bool active) : UIPanel(name,positionX,positionY, width, height, active)
{
}

UIPanelAbout::~UIPanelAbout()
{
}

void UIPanelAbout::Draw(){
	ImGui::Begin(name.c_str(), &active);

	ImGui::Text("Video Game Engine. \n\nThis Engine is done for the 3D Engines subject on the Design & Development Degree by UPC \n\nDeveloped by:\n");
	if (ImGui::SmallButton("Norman Benet")) {
		App->RequestBrowser("https://github.com/normanbg");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	if (ImGui::SmallButton("Carles Margeli")) {
		App->RequestBrowser("https://github.com/Margeli");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	ImGui::Spacing();
	ImGui::Text("Libraries Used:");
	SDL_version version;
	SDL_GetVersion(&version);
	char sdlVersion[30];
	sprintf_s(sdlVersion, 30, "SDL Version: %d.%d.%d", version.major, version.minor, version.patch);
	if (ImGui::SmallButton(sdlVersion)){
		App->RequestBrowser("https://www.libsdl.org/download-2.0.php");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	std::string glVersion = (const char*)glGetString(GL_VERSION);
	glVersion = "OpenGL Version: " + glVersion;
	if (ImGui::SmallButton(glVersion.c_str())){
		App->RequestBrowser("https://www.opengl.org/");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	std::string imguiVersion = ImGui::GetVersion();
	imguiVersion = "ImGui Version: " + imguiVersion;
	if (ImGui::SmallButton(imguiVersion.c_str())){
		App->RequestBrowser("https://github.com/ocornut/imgui");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	if (ImGui::SmallButton("PCG Version: 0.94(C Implementation)")){
		App->RequestBrowser("http://www.pcg-random.org/");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	if (ImGui::SmallButton("MathGeoLIB Version: 1.5")){
		App->RequestBrowser("https://github.com/juj/MathGeoLib");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	char assimpVersion[30];
	sprintf_s(assimpVersion, 30, "Assimp Version: %d.%d.%d", aiGetVersionMajor(),aiGetVersionMinor(), aiGetVersionRevision());
	if (ImGui::SmallButton(assimpVersion)){
		App->RequestBrowser("http://www.assimp.org/index.php/downloads");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	char devillVersion[30];
	sprintf_s(devillVersion, 30, "DevIL Version %d", App->gui->ilVersion);
	if (ImGui::SmallButton(devillVersion)){
		App->RequestBrowser("http://openil.sourceforge.net/download.php");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	std::string glewVersion = (const char*)glewGetString(GLEW_VERSION);
	glewVersion = "Glew Version: " + glewVersion;
	if (ImGui::SmallButton(glewVersion.c_str())){
		App->RequestBrowser("https://www.opengl.org/");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	if (ImGui::SmallButton("JSON PARSON")){
		App->RequestBrowser("http://kgabis.github.com/parson/");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	if (ImGui::SmallButton("DeviceId")) {
		App->RequestBrowser("https://github.com/GameTechDev/gpudetect/blob/master/DeviceId.h");
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("License:");
	if (ImGui::SmallButton("MIT")){
		App->RequestBrowser("https://github.com/Normanbg/3dEngine/blob/master/LICENSE");
		App->audio->PlayFx(App->gui->searchingFX);
	}
	ImGui::End();
}
