#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "Camera.h"
#include "ModuleGui.h"
#include "ModuleScene.h"
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

	importer = new SceneImporter();
	texImporter = new TextureImporter();	

	importer->Init();
	texImporter->Init();
	
	json_object_clear(obj);//clear obj to free memory
	return ret;
}

bool ModuleRenderer3D::Start() {
	BROFILER_CATEGORY("Renderer3D_Start", Profiler::Color::HotPink);

	bool ret = true;
	int width = 0;
	int height = 0;
	App->window->GetSize(width, height);
	OnResize(width, height);
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Renderer3D_PreUpdate", Profiler::Color::HotPink);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	
	glMatrixMode(GL_MODELVIEW);
	float* vm = App->camera->cameraComp->GetViewMatrix();
	glLoadMatrixf(vm);

	// light 0 on cam pos
	lights[0].SetPos(App->camera->cameraComp->camRes->frustum.pos.x, App->camera->cameraComp->camRes->frustum.pos.y, App->camera->cameraComp->camRes->frustum.pos.z);

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
	glBindBuffer(GL_ARRAY_BUFFER, 0); ///
	

	//DrawMeshes();
	App->scene->Draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	App->gui->Draw();	

	SDL_GL_SwapWindow(App->window->window); 
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	BROFILER_CATEGORY("Renderer3D_CleanUp", Profiler::Color::HotPink);
	OWN_LOG("Destroying 3D Renderer");

	importer->CleanUp();
		
	SDL_GL_DeleteContext(context); 
	return true;
}

void ModuleRenderer3D::OnResize(const int width, const int height)
{
//---------
	//uint fbo;
	//glGenFramebuffers(1, &fbo);

	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDeleteFramebuffers(1, &fbo);
//---------
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &rbo);

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		OWN_LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, width, height);
	App->camera->UpdateProjMatrix();
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
	App->scene->SetWireframe(active);
	_wireframe = active;
}
void ModuleRenderer3D::SetBoundingBox(bool active){
	_bBox = active;
	App->scene->SetBoundingBox(active);
}

void ModuleRenderer3D::LoadDroppedFBX(char * droppedFileDir){
	//ClearSceneMeshes();
	importer->ImportFBXandLoad(droppedFileDir);	
	//App->camera->FocusToMeshes();
}



/*
void ModuleRenderer3D::ClearSceneMeshes() {

	/*OWN_LOG("Clearing meshes in scene")
		for (int i = meshes.size() - 1; i >= 0; i--) {
			meshes[i].CleanUp();
		}
	meshes.clear();
	for (int i = textures.size() - 1; i >= 0; i--) {
		textures[i].CleanUp();
	}
	textures.clear();
}*/


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

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();
	glLineWidth(1.0f);
}

void ModuleRenderer3D::ShowGrid() {

	glLineWidth(1.0f);
	bool recordedTex = GetTexture2D();
	SetTexture2D(false);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);

	const float d = 100.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

	SetTexture2D(recordedTex);
}
/*


float3 Mesh::getMiddlePoint()const {
	float3 ret = { (boundingBox.maxPoint.x + boundingBox.minPoint.x) / 2, (boundingBox.maxPoint.y + boundingBox.minPoint.y) / 2, (boundingBox.maxPoint.z + boundingBox.minPoint.z) / 2 };

	return ret;
}
*/
