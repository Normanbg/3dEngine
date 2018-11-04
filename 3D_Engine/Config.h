#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "Globals.h"
#include "Math.h"
#include "JSON/parson.h"


class Config
{
public:
	Config();
	Config(const char * jsonName);
	Config(JSON_Object * section);
	~Config();

	uint Save(char ** buffer) const;	

	float GetFloat(const char * field, float default, int index) const;
	bool GetBool(const char * field, bool default, int index) const;
	int GetInt(const char * field, int default, int index) const;
	uint GetUInt(const char * field, uint default, int index) const;
	const char * GetString(const char * field, const char * default, int index) const;
	Config GetArray(const char * field, int index) const;


	bool AddFloat(const char * field, float value);
	bool AddBool(const char * field, bool value);
	bool AddInt(const char * field, int value);
	bool AddUInt(const char * field, uint value);
	bool AddString(const char * field, const char * string);
	bool AddArray(const char * array_name);


	
private:

	JSON_Value * FindValue(const char * field, int index) const;
	
private: 

	JSON_Value* valueRoot = nullptr;
	JSON_Object* root = nullptr;
	JSON_Array* array = nullptr;

	bool to_delete = false;
};

#endif //__APPLICATION_H__
