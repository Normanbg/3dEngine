#ifndef __RENDERERER3D_H__
#define __RENDERERER3D_H__


#include "Module.h"
#include "Light.h"
#include "MathGeoLib\Math\MathAll.h"
#include "SceneImporter.h"
#include "TextureImporter.h"
#include "MathGeoLib/Geometry/AABB.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Globals.h"


//#include "ComponentMesh.h"


#include <array>
#include <vector>
#define MAX_LIGHTS 8
/*
struct Texture {

	uint texWidth = 0;
	uint texHeight = 0;

	std::string path;

	GLuint textureID = -1;

	//void CleanUp();
};

struct Mesh {

	float3 position;
	float3 scale;
	Quat rotation;

	uint id_index = -1;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = -1;
	uint num_vertex = 0;
	float3* vertex = nullptr;

	uint id_normals = -1;
	uint num_normals = 0;
	float3* normals = nullptr;

	uint num_faces = 0; // assume each face is a triangle
	std::string name;
	vec3 colors = { 0,0,0 };

	
	uint num_textureCoords = 0;
	float2* texturesCoords = nullptr;

	GLuint texID = 0;

	AABB boundingBox;

	bool bBox = false;
	bool generated = false;

	void Draw();	
	void CleanUp();	
		
	void generateBoundingBox();
	float3 getMiddlePoint()const ;

private:
	void DrawBoundingBox();
};
*/
class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_Object* obj);
	
	
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool Load(JSON_Object* data)override;
	bool Save(JSON_Object* data) const;

	void OnResize(const int width, const int height);
	char* GetGraphicsModel() const;
	char* GetGraphicsVendor() const;

public:
	void SetDepthTest(bool active);
	void SetCullFace(bool active);
	void SetLighting(bool active);
	void SetColorMaterial(bool active);
	void SetTexture2D(bool active);
	void SetWireframe(bool active);
	void SetNormals(bool active) { _normals = active; }
	void SetAxis(bool active) { _axis = active; }
	void SetGrid(bool active) { _grid = active; }

	void SetBoundingBox(bool active);

	inline bool GetDepthTest() const { return _depthTest; }
	inline bool GetCullFace() const { return _cullFace; }
	inline bool GetLighting()const { return _lighting; }
	inline bool GetColorMaterial()const { return _colorMaterial; }
	inline bool GetTexture2D()const { return _texture2D; }
	inline bool GetWireframe()const { return _wireframe; }
	inline bool GetNormals()const { return _normals; }
	inline bool GetBoundingBox()const { return _bBox; }

	inline bool GetAxis() const { return _axis; }
	inline bool GetGrid() const { return _grid; }


	/*vec GetAvgPosFromMeshes();*/


	/*GLuint CheckIfImageAlreadyLoaded(const char*);*/

	//inline std::vector<Mesh>* GetMeshesList()  { return &meshes; }
	//inline std::vector<Texture>* GetTexturesList()   { return &textures; }

	//void AddMesh(Mesh* mesh);
	//void AddTexture(Texture* tex);
	//Texture* GetTextureFromID(GLuint id);

	void LoadDroppedFBX(char* droppedFileDir);

	void ClearSceneMeshes();

	
	

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	SceneImporter* importer;
	TextureImporter* texImporter;
	
private:

	void ShowAxis();
	void ShowGrid();
	void SetDataFromJson(JSON_Object* data);
	//void GenBuffFromMeshes();
	//void DrawMeshes();

private:

	bool _vSync;

	bool _depthTest = true;
	bool _lighting = true;
	bool _cullFace = true;
	bool _colorMaterial = true;
	bool _texture2D = true;
	bool _wireframe = false;
	bool _axis = true;
	bool _grid = true;
	bool _normals = false;
	bool _bBox = false;

	//std::vector<Mesh> meshes;	
	//std::vector<Texture> textures;
};

#endif // !__RENDERER3D_H__

