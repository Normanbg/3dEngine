#ifndef __ModuleGui_H__
#define __ModuleGui_H__

#include "Globals.h"
#include "Module.h"
#include <list>
#include <vector>

#include "RandomGenerator/pcg_variants.h"



struct ImGuiIO;
union SDL_Event;

class UIPanel;
class UIPanelAbout;
class UIPanelConfig;
class UIPanelConsole;
class UIPanelInspector;
class UIPanelHierarchy;
class UIPanelScene;
class UIPanelMaterials;
class UIPanelSceneInfo;
class UIPanelAssets;
class UIPanelOptimization;
class UIPanelGame;

class ModuleGui : public Module
{
public:
	
	ModuleGui(bool start_enabled = true);
	~ModuleGui();

	bool  Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);	
	bool CleanUp();

	void Draw();



	void AddConsoleLogs(const char* log) ;
	void ImplGuiInputs(SDL_Event* e)const;
	bool isMouseOnScene() const;
	void MouseOnScene(bool mouseScene);
	bool isMouseOnGame() const;
	void MouseOnGame(bool mouseGame);

public:
	bool demoShowcase = false;
	

	std::list<UIPanel*> uiPanels;
	UIPanelAbout*		panelAbout		= nullptr;
	UIPanelConfig*		panelConfig		= nullptr;
	UIPanelConsole*		panelConsole	= nullptr;
	UIPanelInspector*	panelInspector	= nullptr;
	UIPanelHierarchy*	panelHierarchy	= nullptr;
	UIPanelScene*		panelScene		= nullptr;
	UIPanelMaterials*	panelMaterial	= nullptr;
	UIPanelSceneInfo*	panelSceneInfo  = nullptr;
	UIPanelAssets*		panelAssets		= nullptr;
	UIPanelOptimization*panelOptim		= nullptr;
	UIPanelGame*		panelGame		= nullptr;

	std::vector<std::string> logsBuffer;
	uint ilVersion = 0;

	uint dropFX = 0;
	uint closeFX = 0;
	uint openFX = 0;
	uint searchingFX = 0; //Used when open an URL
	ImGuiIO* io;

	bool draw_menu = true;
	bool clearScene = false;
	bool loadFile = false;
private:

	void SetWinDockInv();
private:
	bool mouseOnScene = false;
	bool mouseOnGame = false;
};

#endif 