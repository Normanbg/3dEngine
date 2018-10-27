#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "Glew/libx86/glew32.lib")  
#pragma comment (lib, "Assimp/libx86/assimp.lib")


ComponentMesh::ComponentMesh()
{
	type = MESH;
	texID = 1;
	//GenerateBuffer();
}


ComponentMesh::~ComponentMesh()
{
}


update_status ComponentMesh::PreUpdate(float dt)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); ///THIS LINE MAKES ALL DRAW BY RESETING THE BUFFER, NEEDED???????

	return update_status();
}

bool ComponentMesh::Update()
{
	//Draw();
	
	return true	;
}

void ComponentMesh::CleanUp()
{
	if (id_index != -1)
		glDeleteBuffers(1, &id_index);
	if (id_vertex != -1)
		glDeleteBuffers(1, &id_vertex);
	if (id_normals != -1)
		glDeleteBuffers(1, &id_normals);

	delete[] index;
	delete[] vertex;
	delete[] normals;
	delete[] texturesCoords;
}

void ComponentMesh::GenerateBuffer()
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

void ComponentMesh::Draw()
{
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	

	//glColor3f(colors.x, colors.y, colors.z);
	
	if (num_index == 0) {// if the mesh has no index
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, num_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	}
	else {
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexCoordPointer(2, GL_FLOAT, 0, &(texturesCoords[0]));		

		glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);		

		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_TEXTURE_2D);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (App->renderer3D->GetNormals()) {
		for (int j = 0; j < num_normals; j++) {
			glBegin(GL_LINES);
			glVertex3f(vertex[j].x, vertex[j].y, vertex[j].z);
			glVertex3f(vertex[j].x - normals[j].x, vertex[j].y - normals[j].y, vertex[j].z - normals[j].z);
			glLineWidth(1.0f);
			glEnd();
		}
	}
	/*if (bBox) {
		DrawBoundingBox();
	}*/
}

