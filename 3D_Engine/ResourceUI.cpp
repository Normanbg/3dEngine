#include "ResourceUI.h"


#include "mmgr/mmgr.h"


ResourceUI::ResourceUI(uuid UUID) : ResourceTexture(UUID) 
{
	type = UI;
}


ResourceUI::~ResourceUI()
{
}

void ResourceUI::Save(Config & config) const
{
}

void ResourceUI::Load(const Config & config)
{
	if (exportedFile.size() == 0) {
		exportedFile = LIB_UI_PATH + fileName + DDS_FORMAT;

	}
}
