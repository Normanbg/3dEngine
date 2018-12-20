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

class FBO;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_Object* obj);
	
	
	bool Start();
	update_status PreUpdate(float dt);
	void CreateGameTexture();
	update_status PostUpdate(float dt);
	bool CleanUp();

	bool LoadSettings(Config* data)override;
	bool SaveSettings(Config* data) const override;

	void OnResize(const int width, const int height, bool isScene);
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
	void SetQuadTree(bool active) { _quadtree = active; };
	void SetRay(bool active) { _ray = active; };
	void SetBoundingBox(bool active);


	//void ManageDroppedPEI(char* droppedFileDir);

	inline const bool GetDepthTest() const { return _depthTest; }
	inline const bool GetCullFace() const { return _cullFace; }
	inline const bool GetLighting()const { return _lighting; }
	inline const bool GetColorMaterial()const { return _colorMaterial; }
	inline const bool GetTexture2D()const { return _texture2D; }
	inline const bool GetWireframe()const { return _wireframe; }
	inline const bool GetNormals()const { return _normals; }
	inline const bool GetBoundingBox()const { return _bBox; }
	inline const bool GetAxis() const { return _axis; }
	inline const bool GetGrid() const { return _grid; }
	inline const bool GetQuadTree() const { return _quadtree; }
	inline const bool GetRay() const { return _ray; }
	
	const uint GetSceneFBOTexture();
	const uint GetGameFBOTexture();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;


	//-----

	//uint texture;
	//uint framebuffer;
	//uint rbo;
	
private:

	void ShowAxis();
	void ShowGrid();
	void SetDataFromJson(JSON_Object* data);

	void ManageDroppedFBX(const char* droppedFileDir);

protected:

	 void ReceiveEvent(const Event &event) override;

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
	bool _quadtree = false;
	bool _ray = false;

	FBO* sceneFboTex = nullptr;
	FBO* gameFboTex = nullptr;

};

#endif // !__RENDERER3D_H__

