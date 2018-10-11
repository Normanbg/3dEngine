#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
//#include <gl/GL.h>
//#include <gl/GLU.h>
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
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
		box = { vec{-3.0f,3.0f,-3.0f },{-3.0f,3.0f, -1.0f},{	-3.0f, 1.0f, -1.0f},{-1.0f, 1.0f,-3.0f },{-3.0f,3.0f,-3.0f},{-3.0f, 1.0f,-3.0f},
	{-1.0f,3.0f, -1.0f },{	-3.0f,3.0f,-3.0f},{-1.0f,3.0f,-3.0f},{-1.0f, 1.0f,-3.0f},{-1.0f,3.0f,-3.0f},{-3.0f,3.0f,-3.0f},{-3.0f,3.0f,-3.0f},
	{-3.0f, 1.0f, -1.0f},{	-3.0f, 1.0f,-3.0f},{-1.0f,3.0f, -1.0f},{-3.0f,3.0f, -1.0f},{-3.0f,3.0f,-3.0f},{	-3.0f, 1.0f, -1.0f},{-3.0f,3.0f, -1.0f},
	{-1.0f,3.0f, -1.0f},{-1.0f, 1.0f, -1.0f},{-1.0f,3.0f,-3.0f},{-1.0f, 1.0f,-3.0f},{-1.0f,3.0f,-3.0f},{-1.0f, 1.0f, -1.0f},{	-1.0f,3.0f, -1.0f},
	{-1.0f, 1.0f, -1.0f},{	-1.0f, 1.0f,-3.0f},{-3.0f, 1.0f,-3.0f},{	-1.0f,1.0f, -1.0f},{-3.0f, 1.0f,-3.0f},{-3.0f, 1.0f, -3.0f},{-1.0f, 1.0f, -1.0f},
	{-3.0f, 1.0f, -1.0f},{	-1.0f,3.0f, -1.0f} }; // Box divided into triangles, 102 coords of its triangles (6faces* 2triangles* 3vertex* 3coords)

	glGenBuffers(1, (GLuint*) &(buffBoxID));  // generates 1 buffer. then it assign a GLuint to its mem adress.
	glBindBuffer(GL_ARRAY_BUFFER, buffBoxID); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*box.size() * 3, &box[0], GL_STATIC_DRAW); // send the buffer data to VRAM
	
	box2 = { vec{.0f,.0f,.0f},  {1.0f,.0f,.0f} ,{.0f,1.0f,.0f} , {1.0f,1.0f,.0f} , {.0f,.0f,1.0f} , {1.0f,.0f,1.0f} , {.0f,1.0f,1.0f}  ,  {1.0f,1.0f,1.0f} };
	//Box2 divided into the 8 vertex that it has

	glGenBuffers(1, (GLuint*) &(buffBox2ID));
	glBindBuffer(GL_ARRAY_BUFFER, buffBox2ID); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*box2.size()*3 , &box2[0], GL_STATIC_DRAW); 

	boxIndices = {0,1,2 , 1,3,2 , 3,1,5 , 5,7,3 , 7,5,4 , 6,7,4 , 6,4,0, 0,2,6  , 6,2,3 , 6,3,7 , 0,4,5 , 0,5,1 };

	glGenBuffers(1, (GLuint*)&(buffIndicesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * boxIndices.size(),&boxIndices[0], GL_STATIC_DRAW);

	//Plane
	plane = { vec{0.0f, 0.0f, 0.0f},{ 15.0f,0.0f, 0.0f},{ 0.0f, 0.0f, 15.0f }, {	0.0f, 0.0f, 15.0f },{ 15.0f,0.0f,0.0f },{ 15.0f,0.0f, 15.0f } };
	glGenBuffers(1, (GLuint*) &(buffPlaneID));  // generates 1 buffer. then it assign a GLuint to its mem adress.
	glBindBuffer(GL_ARRAY_BUFFER, buffPlaneID); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*plane.size() * 3, &plane[0], GL_STATIC_DRAW);

	//Ray
	ray = { vec{ 0.0f, 0.0f, 0.0f },{ 0.03f,0.0f, 0.0f },{ 0.0f, 0.0f, 105.0f },{ 0.0f, 0.0f, 105.0f },{ 0.03f,0.0f,0.0f },{ 0.03f,0.0f, 105.0f } };
	glGenBuffers(1, (GLuint*) &(buffRayID));  // generates 1 buffer. then it assign a GLuint to its mem adress.
	glBindBuffer(GL_ARRAY_BUFFER, buffRayID); // set the type of buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*ray.size() * 3, &ray[0], GL_STATIC_DRAW);

	//Frustum
	frustum = { vec{ 5.0f,5.0f,5.0f },{ 6.0f,5.0f,5.0f },{ 5.0f,6.0f,5.0f },{ 6.0f,6.0f,5.0f },		{4.75f,4.75f,6.0f },{ 6.25f,4.75f,6.0f },{ 4.75f,6.25f,6.0f },{ 6.25f,6.25f,6.0f } };

	glGenBuffers(1, (GLuint*) &(buffIndicesFrustumID));
	glBindBuffer(GL_ARRAY_BUFFER, buffIndicesFrustumID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*frustum.size() * 3, &frustum[0], GL_STATIC_DRAW);
	
	//---sphere
	float radius = 1;
	float sectors =10;
	float stacks = 10;
	vec position( 0,0, 0);

	float x, y, z, xy;                              // vertex position
	
	float sectorStep = 2 * pi / sectors;
	float stackStep = pi / stacks;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stacks; ++i)
	{
		stackAngle = pi / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		y = position.y+ radius * sinf(stackAngle);              // r * sin(u)

													// add (sectorCount+1) vertices per stack
													// the first and last vertices have same position and normal, but different tex coods
		for (int j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;

			// vertex position (x, y, z)
			z = position.z+ xy  * cosf(sectorAngle);             // r * cos(u) * cos(v)
			x = position.x + xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			sphere.push_back(vec(x, y, z));
						
		}
	}

	glGenBuffers(1, (GLuint*) &(buffsphereID));
	glBindBuffer(GL_ARRAY_BUFFER, buffsphereID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphere.size() * 3, &sphere[0], GL_STATIC_DRAW);



	int k1, k2;
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (sectors + 1);     // beginning of current stack
		k2 = k1 + sectors + 1;      // beginning of next stack

		for (int j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				sphereIndices.push_back(k1);
				sphereIndices.push_back(k2);
				sphereIndices.push_back(k1 + 1);
			}

			if (i != (stacks - 1))
			{
				sphereIndices.push_back(k1 + 1);
				sphereIndices.push_back(k2);
				sphereIndices.push_back(k2 + 1);
			}
		}
	}

	glGenBuffers(1, (GLuint*)&(buffIndicesSphereID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffIndicesSphereID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * sphereIndices.size(), &sphereIndices[0], GL_STATIC_DRAW);
	
	
	importer->LoadFBX("BakerHouse.fbx");
	GenBuffFromMeshes();

	
	//texImporter->LoadCheckeredPlane();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	BROFILER_CATEGORY("Renderer3D_PreUpdate", Profiler::Color::HotPink);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
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
	//direct mode box 
	/*
	glColor4f(1.0f, 1.0f, .0f, 1.0f); // color cyan

	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 10.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 10.0f, 0.0f); glVertex3f(10.0f, 10.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 10.0f, 0.0f); glVertex3f(10.0f, 0.0f, 10.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 10.0f, 0.0f); glVertex3f(10.0f, 10.0f, 10.0f); glVertex3f(10.0f, 0.0f, 10.0f);
	glVertex3f(0.0f, 10.0f, 10.0f); glVertex3f(10.0f, 0.0f, 10.0f); glVertex3f(10.0f, 10.0f, 10.0f);
	glVertex3f(0.0f, 10.0f, 10.0f); glVertex3f(0.0f, 0.0f, 10.0f); glVertex3f(10.0f, 0.0f, 10.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 10.0f); glVertex3f(0.0f, 10.0f, 10.0f);
	glVertex3f(0.0f, 10.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 10.0f, 10.0f);
	glVertex3f(10.0f, 10.0f, 0.0f); glVertex3f(0.0f, 10.0f, 0.0f); glVertex3f(0.0f, 10.0f, 10.0f);
	glVertex3f(0.0f, 10.0f, 10.0f); glVertex3f(10.0f, 10.0f, 10.0f); glVertex3f(10.0f, 10.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, 10.0f); glVertex3f(0.0f, 0.0f, 10.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 10.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(10.0f, 0.0f, 0.0f);
	glEnd();
*/


	///-------------------BOX DRAWING
	glLineWidth(1.0f); 
	glEnableClientState(GL_VERTEX_ARRAY);//enables vertex array


	//---Ray

	glColor4f(.0f, 1.0f, .0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, buffRayID);// sets the type of buffer
	glVertexPointer(3, GL_FLOAT, 0, NULL);  // points the first vertex
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer

	//Frustum
	glColor4f(0.5f, 1.0f, 0.5f, 1.0f);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffIndicesID);
	glVertexPointer(3, GL_FLOAT, 0, &frustum[0]);
	glDrawElements(GL_TRIANGLES, boxIndices.size(), GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//resets the buffer

	glDisableClientState(GL_VERTEX_ARRAY);
	DrawMeshes();

	
	
	//texImporter->DrawCheckeredPlane();
	
	//-------------------------------------------------Need to call Debug Draw


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
	texImporter->CleanUp();
	glDeleteBuffers(1, &buffBoxID);
	glDeleteBuffers(1, &buffBox2ID);
	glDeleteBuffers(1, &buffsphereID);
	glDeleteBuffers(1, &buffPlaneID);
	glDeleteBuffers(1, &buffRayID);
	glDeleteBuffers(1, &buffIndicesID);
	glDeleteBuffers(1, &buffIndicesSphereID);
	glDeleteBuffers(1, &buffIndicesFrustumID);

	ClearSceneMeshes();
		
	SDL_GL_DeleteContext(context); 
	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
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

void ModuleRenderer3D::AddMesh(Mesh*  mesh)
{

	meshes.push_back(*mesh);
}

void ModuleRenderer3D::LoadDroppedFBX(char * droppedFileDir){
	ClearSceneMeshes();
	importer->LoadFBX(droppedFileDir);
	GenBuffFromMeshes();	
}

void ModuleRenderer3D::ClearSceneMeshes(){

	OWN_LOG("Clearing meshes in scene")
	for (int i = meshes.size()-1; i >=0 ; i--) {
		meshes[i].CleanUp();
	}
	meshes.clear();
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
	float d = 100.0f;

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

		glBindTexture(GL_TEXTURE_2D, texture);
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

void Mesh::generateBoundingBox(){

	AABB aabb;

	aabb.SetNegativeInfinity();
	aabb.Enclose((vec*)vertex,num_vertex);
	
	boundingBox = aabb;
}

