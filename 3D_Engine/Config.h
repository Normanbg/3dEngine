#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Globals.h"
#include "Math.h"

class Config
{
public:
	Config();
	Config(const char * jsonName);
	~Config();

	uint Save(char ** buffer) const;

	
private:
	
	JSON_Value* valueRoot = nullptr;
	JSON_Object* root = nullptr;
	JSON_Array* array = nullptr;

	bool to_delete = false;
};

#endif //__APPLICATION_H__
