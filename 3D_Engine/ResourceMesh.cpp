#include "ResourceMesh.h"
#include "ModuleRenderer3D.h"
#include "Config.h"


#include "mmgr/mmgr.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "Glew/libx86/glew32.lib")  
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ResourceMesh::ResourceMesh(uuid UUID) : Resource(UUID, ResType::Mesh)
{
}

ResourceMesh::~ResourceMesh()
{
}

void ResourceMesh::LoadInMemory()
{
	if (!loaded) {
		App->importer->LoadMeshPEI(exportedFile.c_str(), this);
		GenerateBuffersGPU();
		loaded = true;
	}
	references++;
	
}

void ResourceMesh::FreeInMemory()
{
	references--;
	if (references == 0) {
		if (id_index != -1) {
			glDeleteBuffers(1, &id_index);
			id_index = -1;
		}
		if (id_vertex != -1) {
			glDeleteBuffers(1, &id_vertex);
			id_vertex = -1;
		}
		if (id_normals != -1) {
			glDeleteBuffers(1, &id_normals);
			id_normals = -1;
		}
		RELEASE_ARRAY(index);
		RELEASE_ARRAY(vertex);
		RELEASE_ARRAY(normals);
		RELEASE_ARRAY(texturesCoords);

		loaded = false;
	}
}

void ResourceMesh::Save(Config & config) const
{
}

void ResourceMesh::Load(const Config & config)
{
	
}

void ResourceMesh::CleanUp()
{	
	if (loaded) {
		FreeInMemory();
	}
}

void ResourceMesh::GenerateBuffersGPU()
{
	glGenBuffers(1, (GLuint*) &(id_vertex));
	if (num_normals > 0) 
		glGenBuffers(1, (GLuint*) &(id_normals));// generates 1 buffer. then it assign a GLuint to its mem adress.

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*num_vertex, &vertex[0], GL_STATIC_DRAW);

	if (num_normals > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*num_normals, &normals[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	if (num_index > 0) {
		glGenBuffers(1, (GLuint*) &(id_index));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *num_index, &index[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
