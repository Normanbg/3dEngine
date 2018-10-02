#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "MathGeoLib\Math\MathAll.h"
#include "Importer.h"
#include <array>

#include <vector>
using namespace std;

#define MAX_LIGHTS 8

struct Mesh {

	uint id_index = 0;
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float3* vertex = nullptr;
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
	char* GetGraphicsModel();
	char* GetGraphicsVendor();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	void SetDepthTest(bool active);
	void SetCullFace(bool active);
	void SetLighting(bool active);
	void SetColorMaterial(bool active);
	void SetTexture2D(bool active);
	void SetWireframe(bool active);

	void SetAxis(bool active) { _axis = active; }
	void SetGrid(bool active) { _grid = active; }

	inline bool GetDepthTest() const { return _depthTest; }
	inline bool GetCullFace() const { return _cullFace; }
	inline bool GetLighting()const { return _lighting; }
	inline bool GetColorMaterial()const { return _colorMaterial; }
	inline bool GetTexture2D()const { return _texture2D; }
	inline bool GetWireframe()const { return _wireframe; }

	inline bool GetAxis() const { return _axis; }
	inline bool GetGrid() const { return _grid; }

	void AddMesh(Mesh* mesh);

private:

	void ShowAxis();
	void ShowGrid();
	void GetDataFromJson(JSON_Object* data);
	bool _vSync;


	array<vec, 36> box;
	array<vec, 8> box2;
	array<uint, 36> boxIndices;
	array<vec, 6> plane;
	array<vec, 6> ray;
	array<vec, 8> frustum;

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

	Importer importer;
	std::vector<Mesh> meshes;
	
};