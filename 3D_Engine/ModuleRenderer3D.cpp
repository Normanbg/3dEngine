#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"
#include "GameObject.h"
#include "ModuleGui.h"
#include "Brofiler/Brofiler.h"



#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */ 
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")  

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	name = "Render";

	_vSync = VSYNC;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D(){

	delete texImporter;
	delete importer;
}

// Called before render is available
bool ModuleRenderer3D::Init(JSON_Object* obj)
{
	BROFILER_CATEGORY("Renderer3D_Init", Profiler::Color::HotPink);
	OWN_LOG("Creating 3D Renderer context");
	bool ret = true;
	
	if (obj != nullptr) {
		SetDataFromJson(obj);
	}
	
	//Create OGL context 
	context = SDL_GL_CreateContext(App->window->window); 
	if(context == NULL)
	{
		OWN_LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	GLenum err = glewInit();
	if (err != GL_NO_ERROR) {
		OWN_LOG("Glew could not be initiated! GlewError %s \n", glewGetString(err) );
		ret = false;		
	}
	else {
		OWN_LOG("Using Glew %s \n", glewGetString(GLEW_VERSION));

		OWN_LOG("Vendor: %s", glGetString(GL_VENDOR));
		OWN_LOG("Renderer: %s", glGetString(GL_RENDERER));
		OWN_LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		OWN_LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	if(ret == true)
	{
		//Use Vsync
		if(_vSync && SDL_GL_SetSwapInterval(1) < 0)
			OWN_LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix as identity
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			OWN_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			OWN_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f,0.f, 1.f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		
		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			OWN_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		SetDepthTest(true);
		SetCullFace(true);
		lights[0].Active(true);
		SetLighting(true);
		SetColorMaterial(true);
		SetTexture2D(true);
	}
	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	importer = new MeshImporter();
	texImporter = new TextureImporter();	

	importer->InitDebugLog();
	texImporter->Init();
	
	json_object_clear(obj);//clear obj to free memory
	return ret;
}

bool ModuleRenderer3D::Start() {
	BROFILER_CATEGORY("Renderer3D_Start", Profiler::Color::HotPink);

	bool ret = true;

	importer->LoadFBX("BakerHouse.fbx");
	GenBuffFromMeshes();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Renderer3D_PreUpdate", Profiler::Color::HotPink);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	///NEEDS TO BE CHANGED FOR BEING THE CAM AT THE FRUSTUM POS!!!!
	/*ComponentCamera* cam = (ComponentCamera*)App->camera->editorCam_G0->GetComponent(ComponentType::CAMERA);
	glLoadMatrixf(cam->GetViewMatrix());*/

	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	if (_axis) { ShowAxis(); }
	if (_grid) { ShowGrid(); }
	

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Renderer3D_PostUpdate", Profiler::Color::HotPink);

	glBindBuffer(GL_ARRAY_BUFFER, 0); ///THIS LINE MAKES ALL DRAW BY RESETING THE BUFFER, NEEDED???????

	DrawMeshes();
	//ComponentCamera* cam = (ComponentCamera*)App->camera->editorCam_G0->GetComponent(ComponentType::CAMERA);
	//cam->DebugDraw();
	App->gui->Draw();


	SDL_GL_SwapWindow(App->window->window); 
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	BROFILER_CATEGORY("Renderer3D_CleanUp", Profiler::Color::HotPink);
	OWN_LOG("Destroying 3D Renderer");

	importer->EndDebugLog();
	

	ClearSceneMeshes();
		
	SDL_GL_DeleteContext(context); 
	return true;
}


void ModuleRenderer3D::OnResize(const int width, const int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

char* ModuleRenderer3D::GetGraphicsModel() const
{
	return (char*)glGetString(GL_RENDERER);
}

char * ModuleRenderer3D::GetGraphicsVendor() const
{
	return (char*)glGetString(GL_VENDOR);;
}

void ModuleRenderer3D::SetDataFromJson(JSON_Object* data) {

	_vSync = json_object_dotget_boolean(data, "VSync");

}

void ModuleRenderer3D::GenBuffFromMeshes(){
	Mesh* meshIterator;
	for (int i = 0; i < meshes.size(); i++) {
		meshIterator = &meshes[i];
		if (!meshIterator->generated) {
			glGenBuffers(1, (GLuint*) &(meshIterator->id_vertex));  // generates 1 buffer. then it assign a GLuint to its mem adress.
			glBindBuffer(GL_ARRAY_BUFFER, meshIterator->id_vertex); // set the type of buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*meshIterator->num_vertex, &meshIterator->vertex[0], GL_STATIC_DRAW);
			glGenBuffers(1, (GLuint*) &(meshIterator->id_normals));
			glBindBuffer(GL_ARRAY_BUFFER, meshIterator->id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*meshIterator->num_normals, &meshIterator->normals[0], GL_STATIC_DRAW);
			if (meshIterator->num_index > 0) {
				glGenBuffers(1, (GLuint*) &(meshIterator->id_index));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIterator->id_index);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * meshIterator->num_index, &meshIterator->index[0], GL_STATIC_DRAW);
			}
			meshIterator->generated = true;
		}
	}
}

void ModuleRenderer3D::DrawMeshes(){
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw();
	}		
}

bool ModuleRenderer3D::Load(JSON_Object* data) {

	SetDataFromJson(data);

	//Set Vsync to change it in game

	return true;
}
bool ModuleRenderer3D::Save(JSON_Object* data)const {
	json_object_dotset_boolean(data, "Render.VSync", _vSync);
	
	return true;
}

void ModuleRenderer3D::SetDepthTest(bool active) {
	active ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	_depthTest = active;
}
void ModuleRenderer3D::SetCullFace(bool active) {
	active ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	_cullFace = active;
}
void ModuleRenderer3D::SetLighting(bool active) {
	active ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	_lighting = active;
}
void ModuleRenderer3D::SetColorMaterial(bool active) {
	active ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
	_colorMaterial = active;
}
void ModuleRenderer3D::SetTexture2D(bool active) {
	active ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
	_texture2D = active;
}
void ModuleRenderer3D::SetWireframe(bool active) {
	active ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	_wireframe = active;
}

void ModuleRenderer3D::SetBoundingBox(bool active){
	_bBox = active;
	for (int i =  0; i < meshes.size(); i++) {
		meshes[i].bBox = active;
	}

}


float3 ModuleRenderer3D::GetAvgPosFromMeshes() 
{
	float3 sumPoints = float3(0,0,0);
	
	for (int i = 0; i < meshes.size(); i++) {
		sumPoints += meshes[i].getMiddlePoint();
	}
	return { sumPoints.x / meshes.size(), sumPoints.y / meshes.size(), sumPoints.z / meshes.size() };
}

GLuint ModuleRenderer3D::CheckIfImageAlreadyLoaded(const char * _path)
{
	for (int i = 0; i < App->renderer3D->textures.size();i++) {
		if (strcmp(textures[i].path.c_str() ,_path)==0) {
			return textures[i].textureID;
		}
	}
	return -1;
}

void ModuleRenderer3D::AddMesh(Mesh*  mesh)
{
	meshes.push_back(*mesh);
}

void ModuleRenderer3D::AddTexture(Texture*  tex)
{
	textures.push_back(*tex);
}

Texture* ModuleRenderer3D::GetTextureFromID(GLuint id) 
{
	for (int i = 0; i < textures.size(); i++) {
		if (textures[i].textureID = id) {
			return &textures[i];
		}
	}
	OWN_LOG("Error getting texture from ID");
	return nullptr;
}

void ModuleRenderer3D::LoadDroppedFBX(char * droppedFileDir){
	ClearSceneMeshes();
	importer->LoadFBX(droppedFileDir);	
	GenBuffFromMeshes();	
	//App->camera->FocusToMeshes();
}

void ModuleRenderer3D::ClearSceneMeshes(){

	OWN_LOG("Clearing meshes in scene")
	for (int i = meshes.size()-1; i >=0 ; i--) {
		meshes[i].CleanUp();
	}
	meshes.clear();
	for (int i = textures.size() - 1; i >= 0; i--) {
		textures[i].CleanUp();
	}
	textures.clear();
}

ComponentMesh* ModuleRenderer3D::CreateComponentMesh() {
	ComponentMesh* ret;

	return ret;
}

void ModuleRenderer3D::ShowAxis() {

	
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

	glEnd();
	glLineWidth(1.0f);
}

void ModuleRenderer3D::ShowGrid() {

	glLineWidth(1.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	const float d = 100.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

}

void Mesh::Draw()
{

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColor3f(colors.x, colors.y, colors.z);

	if (num_index == 0) {// if the mesh has no index
		glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, num_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		glBindTexture(GL_TEXTURE_2D, texID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
		glVertexPointer(3, GL_FLOAT, 0, &(vertex[0]));		
		glTexCoordPointer(2, GL_FLOAT, 0, &(texturesCoords[0]));
		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
	if (bBox) {
		DrawBoundingBox();
	}
}

void Mesh::DrawBoundingBox(){

	
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

void Mesh::CleanUp(){

	if(id_index!=-1)
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

void Texture::CleanUp() {
	if (textureID != -1)
		glDeleteBuffers(1, &textureID);
}

void Mesh::generateBoundingBox(){

	AABB aabb;

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)vertex,num_vertex);
	
	boundingBox = aabb;
}

float3 Mesh::getMiddlePoint()const {
	float3 ret = { (boundingBox.maxPoint.x + boundingBox.minPoint.x) / 2, (boundingBox.maxPoint.y + boundingBox.minPoint.y) / 2, (boundingBox.maxPoint.z + boundingBox.minPoint.z) / 2 };

	return ret;
}

