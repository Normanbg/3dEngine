#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
//#include <gl/GL.h>
//#include <gl/GLU.h>
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleGui.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */ 
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")  

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	name = "Render";

	_vSync = VSYNC;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JSON_Object* obj)
{
	OWN_LOG("Creating 3D Renderer context");
	bool ret = true;
	
	if (obj != nullptr) {
		GetDataFromJson(obj);
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
		SetCullFace(true); //commented to test the box1 to fully see it (otherwise box has gaps due to the wrong orientation of the tris)
		lights[0].Active(true);
		SetLighting(true);
		SetColorMaterial(true);
		SetTexture2D(true);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	
	json_object_clear(obj);//clear obj to free memory
	return ret;
}

bool ModuleRenderer3D::Start() {

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

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
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
	
	///-------------------BOX DRAWING
	glLineWidth(1.0f); 
	glEnableClientState(GL_VERTEX_ARRAY);//enables vertex array

	//---TO DRAW BOX with glDrawArrays()
	glColor4f(.0f, 1.0f, 1.0f, 1.0f); // color cyan

	glBindBuffer(GL_ARRAY_BUFFER, buffBoxID);// sets the type of buffer
	glVertexPointer(3, GL_FLOAT, 0, NULL);  // points the first vertex
	glDrawArrays(GL_TRIANGLES, 0, 36); //Draw tris in the 36 n� of vertex that a box has (6faces * 2tris * 3vertex)
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer

	//---TO DRAW BOX 2 with glDrawElements()
	glColor4f(1.0f, .0f, 1.0f, 1.0f);// color magenta

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffIndicesID);
	glVertexPointer(3, GL_FLOAT, 0, &box2[0]);
	glDrawElements(GL_TRIANGLES, boxIndices.size(), GL_UNSIGNED_INT,NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//resets the buffer

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);// color white

	glDisableClientState(GL_VERTEX_ARRAY);

	
	///---------------------------

	//---plane
	glLineWidth(1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor4f(.0f, 0.5f, 1.0f, 1.0f); 

	glBindBuffer(GL_ARRAY_BUFFER, buffPlaneID);// sets the type of buffer
	glVertexPointer(3, GL_FLOAT, 0, NULL);  // points the first vertex
	glDrawArrays(GL_TRIANGLES, 0, 6); 
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer

	glDisableClientState(GL_VERTEX_ARRAY);

	//---Ray
	glLineWidth(1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor4f(.0f, 1.0f, .0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, buffRayID);// sets the type of buffer
	glVertexPointer(3, GL_FLOAT, 0, NULL);  // points the first vertex
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //resets the buffer

	glDisableClientState(GL_VERTEX_ARRAY);


	//Debug Draw
	SDL_GL_SwapWindow(App->window->window); 
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	OWN_LOG("Destroying 3D Renderer");

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

char* ModuleRenderer3D::GetGraphicsModel()
{
	return (char*)glGetString(GL_RENDERER);
}

char * ModuleRenderer3D::GetGraphicsVendor()
{
	return (char*)glGetString(GL_VENDOR);;
}

void ModuleRenderer3D::GetDataFromJson(JSON_Object* data) {

	_vSync = json_object_dotget_boolean(data, "VSync");

}

bool ModuleRenderer3D::Load(JSON_Object* data) {

	GetDataFromJson(data);

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
}

void ModuleRenderer3D::ShowGrid() {

	glLineWidth(1.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();

}