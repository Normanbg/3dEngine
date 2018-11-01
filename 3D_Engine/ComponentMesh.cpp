#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "Glew/libx86/glew32.lib")  
#pragma comment (lib, "Assimp/libx86/assimp.lib")


ComponentMesh::ComponentMesh()
{}


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

	material = nullptr;

	delete[] index;
	delete[] vertex;
	delete[] normals;
	delete[] texturesCoords;
}

void ComponentMesh::DrawInspector() {
	ImGui::Separator();
	ImGui::Text("Vertices: %d", num_index);
	//ImGui::Text("Triangles: %d",num_faces);
	ImGui::Text("Indices: %d", num_index);
	ImGui::Text("Normals: %d \n", num_normals);
	
	const char* currentMaterial = NULL;
	if (material != nullptr) {
		currentMaterial = material->texture->name.c_str();
	
	}
	if (ImGui::BeginCombo("Material", currentMaterial)) 
	{
		std::vector<Material*> mat = App->textures->materials; // change by GetMaterials List to initiate it with label NO TEXTURE
		for (int i = 0; i < mat.size(); i++)
		{
			bool is_selected = false;
			if (currentMaterial != nullptr) {
				bool is_selected = (strcmp(currentMaterial, mat[i]->name.c_str()) == 0);
			}
			if (ImGui::Selectable(mat[i]->name.c_str(), is_selected)) {
				currentMaterial = mat[i]->name.c_str();

				ComponentMaterial* compMat = myGO->GetComponentMaterial();
				if ( compMat == nullptr) { //if the GO has already a component Material
					compMat = (ComponentMaterial*)myGO->AddComponent(MATERIAL);
				}
				
				compMat->texture = App->textures->GetMaterialsFromName(currentMaterial);
				SetMaterial(compMat);
				compMat = nullptr;
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}
			}			
		}
		ImGui::EndCombo();
	}
	ImGui::Separator();
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
	GenerateBoundingBox();
}

void ComponentMesh::Draw()
{
	glPushMatrix();
	glMultMatrixf(myGO->transformComp->globalMatrix.Transposed().ptr());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	

	//glColor3f(colors.x, colors.y, colors.z);
	showWireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // wireframe
	
	if (num_index == 0) {// if the mesh has no index
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, num_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	}
	else {
			
		if (material != nullptr) {			
			glBindTexture(GL_TEXTURE_2D, material->texture->textureID);
			glTexCoordPointer(2, GL_FLOAT, 0, &(texturesCoords[0]));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index); // test: before it was 2 lines upper
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);		
		GLenum error = glGetError();
		
		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

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
	glPopMatrix();
	if (showBBox) {
		DrawBoundingBox();
	}
}

void ComponentMesh::GenerateBoundingBox()
{
	AABB aabb;

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertex, num_vertex);

	boundingBox = aabb;
}

void ComponentMesh::DrawBoundingBox()
{


	glLineWidth(2.0f);

	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f); //green

	glVertex3f(boundingBox.CornerPoint(0).x, boundingBox.CornerPoint(0).y, boundingBox.CornerPoint(0).z);
	glVertex3f(boundingBox.CornerPoint(1).x, boundingBox.CornerPoint(1).y, boundingBox.CornerPoint(1).z);

	glVertex3f(boundingBox.CornerPoint(0).x, boundingBox.CornerPoint(0).y, boundingBox.CornerPoint(0).z);
	glVertex3f(boundingBox.CornerPoint(2).x, boundingBox.CornerPoint(2).y, boundingBox.CornerPoint(2).z);

	glVertex3f(boundingBox.CornerPoint(1).x, boundingBox.CornerPoint(1).y, boundingBox.CornerPoint(1).z);
	glVertex3f(boundingBox.CornerPoint(3).x, boundingBox.CornerPoint(3).y, boundingBox.CornerPoint(3).z);

	glVertex3f(boundingBox.CornerPoint(2).x, boundingBox.CornerPoint(2).y, boundingBox.CornerPoint(2).z);
	glVertex3f(boundingBox.CornerPoint(3).x, boundingBox.CornerPoint(3).y, boundingBox.CornerPoint(3).z);

	glVertex3f(boundingBox.CornerPoint(0).x, boundingBox.CornerPoint(0).y, boundingBox.CornerPoint(0).z);
	glVertex3f(boundingBox.CornerPoint(4).x, boundingBox.CornerPoint(4).y, boundingBox.CornerPoint(4).z);

	glVertex3f(boundingBox.CornerPoint(5).x, boundingBox.CornerPoint(5).y, boundingBox.CornerPoint(5).z);
	glVertex3f(boundingBox.CornerPoint(1).x, boundingBox.CornerPoint(1).y, boundingBox.CornerPoint(1).z);


	glVertex3f(boundingBox.CornerPoint(4).x, boundingBox.CornerPoint(4).y, boundingBox.CornerPoint(4).z);
	glVertex3f(boundingBox.CornerPoint(5).x, boundingBox.CornerPoint(5).y, boundingBox.CornerPoint(5).z);

	glVertex3f(boundingBox.CornerPoint(5).x, boundingBox.CornerPoint(5).y, boundingBox.CornerPoint(5).z);
	glVertex3f(boundingBox.CornerPoint(7).x, boundingBox.CornerPoint(7).y, boundingBox.CornerPoint(7).z);

	glVertex3f(boundingBox.CornerPoint(6).x, boundingBox.CornerPoint(6).y, boundingBox.CornerPoint(6).z);
	glVertex3f(boundingBox.CornerPoint(7).x, boundingBox.CornerPoint(7).y, boundingBox.CornerPoint(7).z);

	glVertex3f(boundingBox.CornerPoint(3).x, boundingBox.CornerPoint(3).y, boundingBox.CornerPoint(3).z);
	glVertex3f(boundingBox.CornerPoint(7).x, boundingBox.CornerPoint(7).y, boundingBox.CornerPoint(7).z);

	glVertex3f(boundingBox.CornerPoint(6).x, boundingBox.CornerPoint(6).y, boundingBox.CornerPoint(6).z);
	glVertex3f(boundingBox.CornerPoint(4).x, boundingBox.CornerPoint(4).y, boundingBox.CornerPoint(4).z);

	glVertex3f(boundingBox.CornerPoint(6).x, boundingBox.CornerPoint(6).y, boundingBox.CornerPoint(6).z);
	glVertex3f(boundingBox.CornerPoint(2).x, boundingBox.CornerPoint(2).y, boundingBox.CornerPoint(2).z);


	glEnd();
	glLineWidth(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void ComponentMesh::SetMaterial(ComponentMaterial * tex)
{
	material = tex;
}
