#ifndef __PANELSCENEINFO_H__
#define __PANELSCENEINFO_H__

#define MAX_CHAR 256

#include "UIPanel.h"

class UIPanelSceneInfo : public UIPanel
{
public:
	UIPanelSceneInfo(const char* name, float positionX, float positionY, float width, float height, bool active = false);
	~UIPanelSceneInfo();

	void Draw();

	bool FileState(const char * extension = nullptr, const char * rootFolder = nullptr);
	const char * CloseFileState();

public:
	bool wantToLoadFile = false;
	bool wantToLoadScene = false;
private:
	void DrawDirectoryRecursive(const char* directory, const char* filterExtension);
	void LoadFilePopUp(const char* extensionFilter = nullptr, const char* rootDirectory = nullptr);

	void LoadScenePopUp(const char * extensionFilter, const char * rootDirectory);

private:
	enum
	{
		closed,
		opened,
		toClose
	}fileState = closed;
	bool inModal = false;
	char selectedFile[MAX_CHAR];
	std::string fileStateExtensionFilter;
	std::string fileStateOrigin;


};
#endif // !__PANELSCENEINFO_H__
