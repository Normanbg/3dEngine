#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib\Math\MathAll.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include <array>

#include <vector>
#define MAX_LIGHTS 8



struct Mesh {

	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float3* vertex = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;
	float3* normals = nullptr;

	uint num_faces = 0; // assume each face is a triangle
	std::string name;
	vec3 colors = { 0,0,0 };

	uint texWidth = 0;
	uint texHeight = 0;
	uint num_textureCoords = 0;
	float2* texturesCoords = nullptr;
	GLuint texture = 0;


	bool generated = false;

	void Draw();
	void CleanUp();
};

struct Texture {


};
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

	void OnResize(int width, int height);
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

	inline bool GetDepthTest() const { return _depthTest; }
	inline bool GetCullFace() const { return _cullFace; }
	inline bool GetLighting()const { return _lighting; }
	inline bool GetColorMaterial()const { return _colorMaterial; }
	inline bool GetTexture2D()const { return _texture2D; }
	inline bool GetWireframe()const { return _wireframe; }
	inline bool GetNormals()const { return _normals; }

	inline bool GetAxis() const { return _axis; }
	inline bool GetGrid() const { return _grid; }

	inline std::vector<Mesh>* GetMeshesList()  { return &meshes; }

	void AddMesh(Mesh* mesh);
	void LoadDroppedFBX(char* droppedFileDir);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	
private:

	void ShowAxis();
	void ShowGrid();
	void SetDataFromJson(JSON_Object* data);
	void GenBuffFromMeshes();
	void DrawMeshes();

private:

	bool _vSync;


	std::array<vec, 36> box;
	std::array<vec, 8> box2;
	std::array<uint, 36> boxIndices;
	std::array<vec, 6> plane;
	std::array<vec, 6> ray;
	std::array<vec, 8> frustum;

	std::vector<vec> sphere;
	std::vector<uint> sphereIndices;


	uint buffBoxID = 0;
	uint buffBox2ID = 0;
	uint buffsphereID = 0;
	uint buffPlaneID = 0;
	uint buffRayID = 0;
	uint buffIndicesID = 0;
	uint buffIndicesSphereID = 0; 
	uint buffIndicesFrustumID = 0;
	
	bool _depthTest = true;
	bool _lighting = true;
	bool _cullFace = true;
	bool _colorMaterial = true;
	bool _texture2D = true;
	bool _wireframe = false;
	bool _axis = true;
	bool _grid = true;
	bool _normals = false;

	std::vector<Mesh> meshes;

public:
	
	MeshImporter* importer;
	TextureImporter* texImporter;
	
};