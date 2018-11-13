#ifndef __RESOURCE_AUDIO_H__
#define __RESOURCE_AUDIO_H__

#include "Globals.h"
#include "Resource.h"

struct Config;

class ResourceAudio : public Resource
{

public:
	ResourceAudio(uuid UUID);
	virtual ~ResourceAudio();

	bool LoadInMemory() override;
	void Save(Config& config) const override;
	void Load(const Config& config) override;

};
#endif