#include "Config.h"


#include "mmgr/mmgr.h"

Config::Config()
{
	valueRoot = json_value_init_object();
	root = json_value_get_object(valueRoot);
	to_delete = true;
}


Config::Config(const char* jsonName)
{
	valueRoot = json_parse_string(jsonName);
	if (valueRoot != nullptr) {
		root = json_value_get_object(valueRoot);
		to_delete = true;
	}
}

Config::Config(JSON_Object* section) : root(section)
{}


Config::~Config()
{
	if (to_delete == true) {
		json_value_free(valueRoot);
	}
}

uint Config::Save(char** buffer) const
{
	uint written = json_serialization_size(valueRoot);
	*buffer = new char[written];
	json_serialize_to_buffer(valueRoot, *buffer, written);
	return written;
}

JSON_Value* Config::FindValue(const char * field, int index) const
{
	if (index < 0) {
		return json_object_get_value(root, field);
	}

	JSON_Array* array = json_object_get_array(root, field);
	if (array != nullptr) {
		return json_array_get_value(array, index);
	}
	return nullptr;
}

float Config::GetFloat(const char * field, float default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONNumber) {
		return (float)json_value_get_number(value);
	}

	return default;
}

bool Config::GetBool(const char * field, bool default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONBoolean) {
		return json_value_get_boolean(value) != 0;
	}

	return default;
}

int Config::GetInt(const char * field, int default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONNumber) {
		return (int)json_value_get_number(value);
	}

	return default;
}

uint Config::GetUInt(const char * field, uint default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONNumber) {
		return (uint)json_value_get_number(value);
	}

	return default;
}

const char* Config::GetString(const char * field, const char* default, int index) const
{
	JSON_Value* value = FindValue(field, index);

	if (value && json_value_get_type(value) == JSONString) {
		return json_value_get_string(value);
	}

	return default;
}

uint Config::GetNumElemsArray(const char * field) const
{	
	JSON_Array* array = json_object_get_array(root, field);
	if (array == nullptr) {
		return -1;
	}	
	int num = json_array_get_count(array);
	return num;
}

Config Config::GetArray(const char * field, int index) const
{
	JSON_Array* array = json_object_get_array(root, field);
	if (array != nullptr) {
		return Config(json_array_get_object(array, index));
	}
	return Config((JSON_Object*) nullptr);
}

float2 Config::GetFloat2(const char * field, const float2 & default)
{
	return float2(
		GetFloat(field, default.x, 0),
		GetFloat(field, default.y, 1));
}

float3 Config::GetFloat3(const char * field, const float3 & default)
{
	return float3(
		GetFloat(field, default.x, 0),
		GetFloat(field, default.y, 1),
		GetFloat(field, default.z, 2));
}

float4 Config::GetFloat4(const char * field, const float4 & default)
{
	return float4(
		GetFloat(field, default.x, 0),
		GetFloat(field, default.y, 1),
		GetFloat(field, default.z, 2),
		GetFloat(field, default.w, 3));
}


bool Config::AddBool(const char * field, bool value)
{
	return json_object_set_boolean(root, field, (value) ? 1 : 0) == JSONSuccess;
}

bool Config::AddInt(const char * field, int value)
{
	return json_object_set_number(root, field, (double)value) == JSONSuccess;
}

bool Config::AddUInt(const char * field, uint value)
{
	return json_object_set_number(root, field, (double)value) == JSONSuccess;
}

bool Config::AddFloat(const char * field, float value)
{
	return json_object_set_number(root, field, (float)value) == JSONSuccess;
}

bool Config::AddString(const char * field, const char * string)
{
	return json_object_set_string(root, field, string) == JSONSuccess;
}

bool Config::AddArray(const char* array_name)
{
	JSON_Value* va = json_value_init_array();
	array = json_value_get_array(va);

	return json_object_set_value(root, array_name, va) == JSONSuccess;
}

bool Config::AddArrayChild(const Config & config)
{
	if (array != nullptr) {
		return json_array_append_value(array, json_value_deep_copy(config.valueRoot)) == JSONSuccess;
	}
	return false;
}

bool Config::AddArrayFloat(const char * field, const float * values, int size)
{
	if (values != nullptr && size > 0)
	{
		JSON_Value* va = json_value_init_array();
		array = json_value_get_array(va);
		json_object_set_value(root, field, va);

		for (int i = 0; i < size; ++i) {
			json_array_append_number(array, values[i]);
		}
		return true;
	}
	return false;
}

bool Config::AddFloat2(const char * field, const float2 & value)
{
	return AddArrayFloat(field, &value.x, 2);
}

bool Config::AddFloat3(const char * field, const float3 & value)
{
	return AddArrayFloat(field, &value.x, 3);
}


bool Config::AddFloat4(const char * field, const float4 & value)
{
	return AddArrayFloat(field, &value.x, 4);
}

