#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Globals.h"
#include "Resource.h"

class Config;

class ResourceTexture :	public Resource
{
	enum Format {
		color_index,
		rgb,
		rgba,
		bgr,
		bgra,
		luminance,
		unknown
	};

public:
	ResourceTexture(uuid UUID);
	virtual ~ResourceTexture();

	void LoadInMemory() override;
	void Save(Config& config) const override;
	void Load(const Config& config) override; 

public:
	uint width = 0;
	uint height = 0;
	uint gpu_id = 0;

	uint depth = 0;
	uint mips = 0;
	uint bytes = 0;
	
	Format format = unknown;
};

#endif// !__RESOURCE_TEXTURE_H__
