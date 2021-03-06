#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleEditorCamera.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "Camera.h"
#include "Config.h"
#include "ModuleGui.h"
#include "ModuleScene.h"
#include "Brofiler/Brofiler.h"
#include "FBO.h"

#include "mmgr/mmgr.h"



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
		
}

// Called before render is available
bool ModuleRenderer3D::Init(JSON_Object* obj)
{
	BROFILER_CATEGORY("Renderer3D_Init", Profiler::Color::HotPink);
	OWN_LOG("Creating 3D Renderer context");
	bool ret = true;
	
	
	
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

		sceneFboTex = new FBO();
		gameFboTex = new FBO();
		int w, h;
		App->window->GetSize(w, h);
		sceneFboTex->Create(w , h);
		gameFboTex->Create(w, h);

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

	
	return ret;
}

bool ModuleRenderer3D::Start() {
	BROFILER_CATEGORY("Renderer3D_Start", Profiler::Color::HotPink);

	bool ret = true;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Renderer3D_PreUpdate", Profiler::Color::HotPink);
	
	

	return UPDATE_CONTINUE;
}

void ModuleRenderer3D::CreateGameTexture() {
	gameFboTex->BindFBO();
	if (App->scene->mainCamera == nullptr) { return; }
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	ComponentCamera* mainCam = App->scene->mainCamera->GetComponentCamera();
	glLoadMatrixf(mainCam->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(mainCam->GetViewMatrix());

}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	BROFILER_CATEGORY("Renderer3D_PostUpdate", Profiler::Color::HotPink);
	glBindBuffer(GL_ARRAY_BUFFER, 0); ///
	

	//DrawMeshes();
	SetDepthTest(true);
	SetLighting(true);
	sceneFboTex->BindFBO();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//-
	glLoadMatrixf(App->camera->cameraComp->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	//-

	glLoadMatrixf(App->camera->cameraComp->GetViewMatrix());

	//PreUpdateGame();
	// light 0 on cam pos
	lights[0].SetPos(App->camera->cameraComp->GetPos().x, App->camera->cameraComp->GetPos().y, App->camera->cameraComp->GetPos().z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	if (_axis) { ShowAxis(); }
	if (_grid) { ShowGrid(); }
	bool editor = true;
	App->scene->ToggleEditorCam();
	App->scene->Draw(editor);
	sceneFboTex->UnBindFBO();

	editor = false;
	gameFboTex->BindFBO();
	CreateGameTexture();
	App->scene->Draw(editor);
	App->scene->ToggleEditorCam();
	SetDepthTest(false);
	SetLighting(false);
	App->scene->DrawInGameUI();
	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();
	gameFboTex->UnBindFBO();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	App->gui->Draw();	

	SDL_GL_SwapWindow(App->window->window); 
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	BROFILER_CATEGORY("Renderer3D_CleanUp", Profiler::Color::HotPink);
	OWN_LOG("Destroying 3D Renderer");
		
	sceneFboTex->UnBindFBO();
	gameFboTex->UnBindFBO();
	sceneFboTex->DeleteFBO();
	gameFboTex->DeleteFBO();

	RELEASE(sceneFboTex);
	RELEASE(gameFboTex);

	SDL_GL_DeleteContext(context); 
	return true;
}

void ModuleRenderer3D::OnResize(const int width, const int height, bool scene)
{
	if (scene) {
		App->camera->cameraComp->SetAspectRatio((float)width / (float)height);
	}
	else {
		if (App->scene->mainCamera) {
			ComponentCamera* mainCam = App->scene->mainCamera->GetComponentCamera();
			mainCam->SetAspectRatio((float)width / (float)height);
			GameObject* canvasGO = App->scene->GetFirstGameObjectCanvas();
			if (canvasGO) {
				canvasGO->GetComponentCanvas()->SetResolution(float2(width, height));
			}
		}
	}
}

char* ModuleRenderer3D::GetGraphicsModel() const
{
	return (char*)glGetString(GL_RENDERER);
}

char * ModuleRenderer3D::GetGraphicsVendor() const
{
	return (char*)glGetString(GL_VENDOR);;
}



bool ModuleRenderer3D::LoadSettings(Config* data) {

	data->GetBool("VSync", false);

	return true;
}
bool ModuleRenderer3D::SaveSettings(Config* data)const {
	data->AddBool("VSync", _vSync);
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

void ModuleRenderer3D::ManageDroppedFBX(const char * droppedFileDir){
	App->importer->LoadFBXScene(droppedFileDir);	
}

const uint ModuleRenderer3D::GetSceneFBOTexture()
{
	return sceneFboTex->texture;
}

const uint ModuleRenderer3D::GetGameFBOTexture()
{
	return gameFboTex->texture;
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

void ModuleRenderer3D::ReceiveEvent(const Event & event)
{
	switch (event.type) {
	
	case Event::EventType::scene_file_dropped: {
		ManageDroppedFBX(event.string);
		break;
	}
	
	}
}

/*


float3 Mesh::getMiddlePoint()const {
	float3 ret = { (boundingBox.maxPoint.x + boundingBox.minPoint.x) / 2, (boundingBox.maxPoint.y + boundingBox.minPoint.y) / 2, (boundingBox.maxPoint.z + boundingBox.minPoint.z) / 2 };

	return ret;
}
*/
