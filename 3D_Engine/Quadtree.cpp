#include "Quadtree.h"
#include "GameObject.h"
#include "ComponentMesh.h"

#include <vector>

Quadtree::Quadtree(AABB limits, uint subdivision)
{
	quadTreeBox = limits;
	subdivisions = subdivision;
}

Quadtree::~Quadtree()
{
}

void Quadtree::Clear(){
	quadTreeBox.SetNegativeInfinity();
	subdivisions = 0;
	if (!quTrChilds.empty()) {
		for (auto it : quTrChilds)
		{
			it->Clear();
			it = nullptr;
		}
		quTrChilds.clear();
	}
	for (auto j : gameobjs)
	{
		RELEASE(j);
	}
	gameobjs.clear();
}

void Quadtree::Insert(GameObject * gameobject) {
	//if (gameobject->GetComponentMesh() == nullptr)
	//{
	//	OWN_LOG("%s require a mesh for the Bounding Box", gameobject->name.c_str());
	//	return;
	//}

	if (quadTreeBox.Intersects(gameobject->globalAABB)) {
		if (!quTrChilds.empty()) {
			for (auto qtChildIt : quTrChilds){
				qtChildIt->Insert(gameobject);
			}
		}
		else if (subdivisions > MAX_SUBDIVISIONS) {
			gameobjs.push_back(gameobject);
			return;
		}
		else {

			gameobjs.push_back(gameobject);
			if (QT_MAX_ELEMS < gameobjs.size())
			{
				if (quTrChilds.empty())
					Subdivide();

				for (auto gobjsIt : gameobjs)
				{
					for (auto childsIt : quTrChilds)
						childsIt->Insert(gobjsIt);
				}

				gameobjs.clear();
			}
			
		}
	}
}

inline void Quadtree::Intersect(std::vector<GameObject*>& objects, const AABB& primitive) const
{
	if (primitive.Intersects(quadTreeBox))
	{
		for (std::vector<GameObject*>::const_iterator it = this->gameobjs.begin(); it != this->gameobjs.end(); ++it)
		{
			if (primitive.Intersects((*it)->globalAABB))
				objects.push_back(*it);
		}
		for (int i = 0; i < 4; ++i)
			if (quTrChilds[i] != nullptr) quTrChilds[i]->Intersect(objects, primitive);
	}
}

void Quadtree::Subdivide(){
	float3 centerXZ = { quadTreeBox.HalfSize().x, quadTreeBox.Size().y, quadTreeBox.HalfSize().z };
	
	//nw
	quTrChilds.push_back(new Quadtree(AABB(quadTreeBox.minPoint, quadTreeBox.minPoint + centerXZ), subdivisions + 1));

	//ne
	float3 neMin = quadTreeBox.minPoint + float3(centerXZ.x, 0.f, 0.f);
	quTrChilds.push_back(new Quadtree(AABB(neMin, neMin + centerXZ), subdivisions + 1));

	//sw 
	float3 swMin = quadTreeBox.minPoint + float3(0.f, 0.f, centerXZ.z);
	quTrChilds.push_back(new Quadtree(AABB(swMin, swMin + centerXZ), subdivisions + 1));

	//se
	quTrChilds.push_back(new Quadtree(AABB(float3(centerXZ.x, 0.f, centerXZ.z), quadTreeBox.maxPoint), subdivisions + 1));
	
}

void Quadtree::AddGOtoQuadtree(GameObject * go)
{
	if (go == nullptr)
		return;

	if (go->GetComponentMesh()) {
		this->Insert(go);
	}
	for (auto it : go->childrens)
		AddGOtoQuadtree(it);
}

void Quadtree::SetSize(GameObject* go) {
	if (go == nullptr)
		return;
	if (go->staticGO)
	{
		quadTreeBox.Enclose(go->globalAABB);
		for (auto it : go->childrens)
		{
			SetSize(it);
		}
	}
}


void Quadtree::DebugDraw(){

	float3 corners[8];
	quadTreeBox.GetCornerPoints(corners);
	DebugDrawBox(corners, Red, 5.f);

	for (auto it : quTrChilds)
	{
		it->DebugDraw();
	}
}
