#ifndef __RESOURCEUI_H__
#define __RESOURCEUI_H__

#include "Globals.h"
#include "ResourceTexture.h"
class ResourceUI :	public ResourceTexture
{
public:
	ResourceUI(uuid UUID);
	~ResourceUI();

	void Save(Config& config) const override;
	void Load(const Config& config) override;

};

#endif // __RESOURCEUI_H__
