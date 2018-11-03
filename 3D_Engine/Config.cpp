#include "Config.h"
#include "JSON/parson.h"


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


Config::~Config()
{
	if (to_delete == true)
		json_value_free(valueRoot);
}

uint Config::Save(char** buffer) const
{
	uint written = json_serialization_size(valueRoot);
	*buffer = new char[written];
	json_serialize_to_buffer(valueRoot, *buffer, written);
	return written;
}