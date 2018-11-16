#include "Application.h"
#include "ComponentMesh.h"
#include "ResourceMesh.h"
#include "ModuleRenderer3D.h"
#include "ModuleResources.h"
#include "GameObject.h"

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "Glew/libx86/glew32.lib")  
#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "mmgr/mmgr.h"


ComponentMesh::ComponentMesh()
{
	type = MESH;

}


ComponentMesh::~ComponentMesh()
{
}


update_status ComponentMesh::PreUpdate(float dt)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); ///

	return UPDATE_CONTINUE;
}



bool ComponentMesh::Update()
{
	return true	;
}

void ComponentMesh::CleanUp()
{
	resourceMesh->FreeInMemory();
	resourceMesh = nullptr;

	material = nullptr;

}

void ComponentMesh::DrawInspector() {
	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.25f, 0.25f, 0.25f, 1), "UUID: %i", GetUUID());
	ImGui::Text("Resource UUID: %i", resourceMesh->GetUUID());
	ImGui::Text("");
	ImGui::Text("Vertices: %d", resourceMesh->num_index);
	ImGui::Text("Triangles: %d", resourceMesh->num_faces);
	ImGui::Text("Indices: %d", resourceMesh->num_index);
	ImGui::Text("Normals: %d \n", resourceMesh->num_normals);

	const char* currentMaterial = NULL;
	if (material != nullptr) {
		currentMaterial = material->GetTextureName();
	
	}
	if (ImGui::BeginCombo("Material", currentMaterial)) 
	{
		std::vector<Resource*> mat = App->resources->GetResourcesListType(Resource::ResType::Texture);
			
		// change by GetMaterials List to initiate it with label NO TEXTURE
		for (int i = 0; i < mat.size(); i++)
		{
			bool is_selected = false;
			if (currentMaterial != nullptr) {
				bool is_selected = (strcmp(currentMaterial, mat[i]->GetName()) == 0);
			}
			if (ImGui::Selectable(mat[i]->GetName(), is_selected)) {
				currentMaterial = mat[i]->GetName();
				ComponentMaterial* compMat = myGO->GetComponentMaterial();

				if (compMat == nullptr) { //if the GO has already a component Material
					compMat = (ComponentMaterial*)myGO->AddComponent(MATERIAL);
				}

				//compMat->texture = App->textures->GetMaterialsFromName(currentMaterial);
				compMat->SetResource(App->resources->FindByName(mat[i]->GetName(), Resource::ResType::Texture));
				SetMaterial(compMat);
				
				compMat = nullptr;
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}

			}
		
			
		}
		ImGui::EndCombo();
	}
	currentMaterial = nullptr;
	ImGui::Separator();
}
/*
void ComponentMesh::GenerateBuffer()
{
	
	
	//GenerateBoundingBox();
}*/

void ComponentMesh::SetResource(uuid resource)
{
	resourceMesh = (ResourceMesh*) App->resources->Get(resource);
	resourceMesh->LoadInMemory();
}



const bool ComponentMesh::HasMesh() const
{
	bool ret;
	resourceMesh ? ret = true : ret = false;
	return ret;

}

void ComponentMesh::Draw()
{
	glPushMatrix();
	glMultMatrixf(myGO->transformComp->getGlobalMatrix().Transposed().ptr());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	

	//glColor3f(colors.x, colors.y, colors.z);
	if (myGO->GetSelected() && drawWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		
	}
	if (showWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);// wireframe
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	
	if (resourceMesh->num_index == 0) {// if the mesh has no index
		glBindBuffer(GL_ARRAY_BUFFER, resourceMesh->id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, resourceMesh->num_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	}
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if (material != NULL){
			if (material->HasTexture() ) {
				glBindTexture(GL_TEXTURE_2D, material->GetTexID());
				glTexCoordPointer(2, GL_FLOAT, 0, &(resourceMesh->texturesCoords[0]));
			}
			else {
				glColor3f(material->colors.x, material->colors.y, material->colors.z);
			}
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resourceMesh->id_index); // test: before it was 2 lines upper
		glBindBuffer(GL_ARRAY_BUFFER, resourceMesh->id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);		
		glDrawElements(GL_TRIANGLES, resourceMesh->num_index, GL_UNSIGNED_INT, NULL);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (App->renderer3D->GetNormals()) {
		for (int j = 0; j < resourceMesh->num_normals; j++) {
			glBegin(GL_LINES);
			glVertex3f(resourceMesh->vertex[j].x, resourceMesh->vertex[j].y, resourceMesh->vertex[j].z);
			glVertex3f(resourceMesh->vertex[j].x - resourceMesh->normals[j].x, resourceMesh->vertex[j].y - resourceMesh->normals[j].y, resourceMesh->vertex[j].z - resourceMesh->normals[j].z);
			glLineWidth(1.0f);
			glEnd();
		}
	}
	glPopMatrix();
	if (showBBox) {
		DrawBoundingBox();
	}
}

void ComponentMesh::Save(Config & data) const
{
	data.AddUInt("UUID", UUID);
	if (material != nullptr) {
		const uint texuuid = material->GetUUID();
		data.AddUInt("TexUUID", texuuid);
	}
	if (resourceMesh!=nullptr) {
		data.AddString("NamePEI", resourceMesh->GetName());
	}
}

void ComponentMesh::Load(Config * data)
{
	UUID = data->GetUInt("UUID");
	SetMaterial(myGO->GetComponentMaterial(data->GetUInt("TexUUID")));
	resourceMesh = (ResourceMesh*) App->resources->Get(App->resources->FindByName(data->GetString("NamePEI"), Resource::ResType::Mesh));
	resourceMesh->LoadInMemory();
	//App->renderer3D->importer->LoadMeshPEI(this);
}


void ComponentMesh::DrawBoundingBox()
{
	float3 corners[8];
	myGO->globalAABB.GetCornerPoints(corners);
	DebugDrawBox(corners);
}

void ComponentMesh::CreateBBox()
{
	float minBoundingX = resourceMesh->vertex->x;
	float maxBoundingX = resourceMesh->vertex->x;
	float minBoundingY = resourceMesh->vertex->y;
	float maxBoundingY = resourceMesh->vertex->y;
	float minBoundingZ = resourceMesh->vertex->z;
	float maxBoundingZ = resourceMesh->vertex->z;

	for (int i = 0; i < resourceMesh->num_vertex; i++) {
		if (resourceMesh->vertex[i].x < minBoundingX)
			minBoundingX = resourceMesh->vertex[i].x;
		else if (resourceMesh->vertex[i].x > maxBoundingX)
			maxBoundingX = resourceMesh->vertex[i].x;
		if (resourceMesh->vertex[i].y < minBoundingY)
			minBoundingY = resourceMesh->vertex[i].y;
		else if (resourceMesh->vertex[i].y > maxBoundingY)
			maxBoundingY = resourceMesh->vertex[i].y;
		if (resourceMesh->vertex[i].z < minBoundingZ)
			minBoundingZ = resourceMesh->vertex[i].z;
		else if (resourceMesh->vertex[i].z > maxBoundingZ)
			maxBoundingZ = resourceMesh->vertex[i].z;
	}
	bbox.minPoint = float3(minBoundingX, minBoundingY, minBoundingZ);
	bbox.maxPoint = float3(maxBoundingX, maxBoundingY, maxBoundingZ);
}


void ComponentMesh::SetMaterial(ComponentMaterial * tex)
{
	material = tex;
}
