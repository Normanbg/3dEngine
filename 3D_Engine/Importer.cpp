#include "Importer.h"



Importer::Importer()
{
}


Importer::~Importer()
{
}

void Importer::Log(){
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}
