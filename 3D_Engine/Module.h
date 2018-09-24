#pragma once


#include "./JSON/parson.h"
#include <string>


class Application;
struct PhysBody3D;


class Module
{
private :
	bool enabled;

public:
	Application* App;
	std::string name;

	Module(Application* parent, bool start_enabled = true) : App(parent)
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

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual bool Load(JSON_Object* data)
	{
		return true;
	}

	virtual bool Save(JSON_Object* data) const
	{
		return true;
	}
};