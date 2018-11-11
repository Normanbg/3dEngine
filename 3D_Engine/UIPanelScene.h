#ifndef __PANELSCENE_H__
#define __PANELSCENE_H__

#include "UIPanel.h"
#include "Math.h"

#define FILE_MAX 256

class ComponentMaterial;


class UIPanelScene : public UIPanel
{
public:
	UIPanelScene(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelScene();

	void Draw();
	void ClearScenePopUp();
	
	bool FileState(const char * extension = nullptr, const char * rootFolder= nullptr);
	const char * CloseFileState();
public:
	ImTextureID img;
	//ComponentMaterial* playButtonMat;
	//ComponentMaterial* pauseButtonMat;
	//ComponentMaterial* stopButtonMat;

	float2 lastSize;
	float2 size;

	bool wantToLoadFile = false;

private:
	enum
	{
		closed,
		opened,
		toClose
	}fileState = closed;
	bool inModal= false;
	char selectedFile[FILE_MAX];
	std::string fileStateExtensionFilter;
	std::string fileStateOrigin;

private: 

	void DrawDirectoryRecursive(const char* directory, const char* filterExtension);
	void LoadFilePopUp( const char* extensionFilter = nullptr, const char* rootDirectory = nullptr);
	

};
#endif // !__PANELSCENE_H__
