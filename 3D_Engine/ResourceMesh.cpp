#include "ResourceMesh.h"
#include "ModuleRenderer3D.h"
#include "Config.h"


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
		App->renderer3D->importer->LoadMeshPEI(exportedFile.c_str(), this);
		GenerateBuffersGPU();
		loaded = true;
	}
	references++;
	
}

void ResourceMesh::UnloadInMemory()
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
		index = nullptr;
		vertex = nullptr;
		normals = nullptr;
		texturesCoords = nullptr;

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
	UnloadInMemory();
	

}

void ResourceMesh::GenerateBuffersGPU()
{
	glGenBuffers(1, (GLuint*) &(id_vertex));
	glGenBuffers(1, (GLuint*) &(id_normals));// generates 1 buffer. then it assign a GLuint to its mem adress.

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*num_vertex, &vertex[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*num_normals, &normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (num_index > 0) {
		glGenBuffers(1, (GLuint*) &(id_index));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *num_index, &index[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
