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

	float GetFloat(const char * field, float default = 0.0f, int index = -1) const;
	bool GetBool(const char * field, bool default, int index = -1) const;
	int GetInt(const char * field, int default = 0, int index = -1) const;
	uint GetUInt(const char * field, uint default = 0, int index = -1) const;
	const char * GetString(const char * field, const char * default= nullptr, int index = -1) const;
	uint GetNumElemsArray(const char * field) const;
	Config GetArray(const char * field, int index ) const;
	float3 GetFloat3(const char * field, const float3 & default);
	float4 GetFloat4(const char * field, const float4 & default);


	bool AddFloat(const char * field, float value);
	bool AddBool(const char * field, bool value);
	bool AddInt(const char * field, int value);
	bool AddUInt(const char * field, uint value);
	bool AddString(const char * field, const char * string);
	bool AddArray(const char * array_name);
	bool AddArrayChild(const Config & config);
	bool AddArrayFloat(const char * field, const float * values, int size);
	bool AddFloat3(const char * field, const float3 & value);
	bool AddFloat4(const char * field, const float4 & value);



	
private:

	JSON_Value * FindValue(const char * field, int index) const;
	
private: 

	JSON_Value* valueRoot = nullptr;
	JSON_Object* root = nullptr;
	JSON_Array* array = nullptr;

	bool to_delete = false;
};

#endif //__APPLICATION_H__
