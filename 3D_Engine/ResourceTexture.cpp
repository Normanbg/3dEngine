#include "ResourceTexture.h"
#include "Config.h"
#include "ModuleRenderer3D.h"




ResourceTexture::ResourceTexture(uuid UUID) : Resource(UUID, ResType::Texture)
{
}

ResourceTexture::~ResourceTexture()
{
}

void ResourceTexture::LoadInMemory()
{
	if (!loaded) {
		gpuID = App->texImporter->LoadTexture(exportedFile.c_str(), width, height);
		loaded = true;
	}
	references++;

}

void ResourceTexture::FreeInMemory()
{
	references--;
	if (references == 0) {
	if(gpuID!=-1)
	glDeleteBuffers(1, &gpuID);
	loaded = false;
	}
}

void ResourceTexture::Save(Config & config) const
{
}

void ResourceTexture::Load(const Config & config)
{
}

void ResourceTexture::CleanUp()
{
}
