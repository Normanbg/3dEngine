#ifndef __RENDERERER3D_H__
#define __RENDERERER3D_H__


#include "Module.h"
#include "Light.h"
#include "SceneImporter.h"
#include "TextureImporter.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Globals.h"

#include <array>
#include <vector>
#define MAX_LIGHTS 8

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
	void SetLoadFBXTest(bool active) { _loadFBXTest = active; }
	void SetBoundingBox(bool active);

	inline bool GetDepthTest() const { return _depthTest; }
	inline bool GetCullFace() const { return _cullFace; }
	inline bool GetLighting()const { return _lighting; }
	inline bool GetColorMaterial()const { return _colorMaterial; }
	inline bool GetTexture2D()const { return _texture2D; }
	inline bool GetWireframe()const { return _wireframe; }
	inline bool GetNormals()const { return _normals; }
	inline bool GetBoundingBox()const { return _bBox; }
	inline bool GetLoadFBXTest()const { return _loadFBXTest; }
	inline bool GetAxis() const { return _axis; }
	inline bool GetGrid() const { return _grid; }


	/*vec GetAvgPosFromMeshes();*/
	   
	void LoadDroppedFBX(char* droppedFileDir);

	//void ClearSceneMeshes();

	
	

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	SceneImporter* importer;
	TextureImporter* texImporter;
	
private:

	void ShowAxis();
	void ShowGrid();
	void SetDataFromJson(JSON_Object* data);

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
	bool _loadFBXTest = false;

};

#endif // !__RENDERER3D_H__

