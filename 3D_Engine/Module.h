#ifndef __MODULE_H__
#define __MODULE_H__

#include "./JSON/parson.h"
#include "Config.h"
#include <string>

class Application;


class Module
{
private:
	bool enabled;

public:

	std::string name;

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init(JSON_Object* obj)
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadSettings(Config* data)
	{
		return true;
	}

	virtual bool SaveSettings(Config* data) const
	{
		return true;
	}
};

#endif // !__MODULE_H__
