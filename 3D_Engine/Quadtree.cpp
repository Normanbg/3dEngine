#include "Quadtree.h"
#include "GameObject.h"
#include "ComponentMesh.h"

#include "mmgr/mmgr.h"

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
		j = nullptr;
	}
	gameobjs.clear();
}

void Quadtree::Insert(GameObject * gameobject) {
	if (gameobject->GetComponentMesh() == nullptr)
	{
		OWN_LOG("%s require a mesh for the Bounding Box", gameobject->name.c_str());
		return;
	}

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

void Quadtree::Intersect(std::vector<GameObject*>& objects, const AABB& primBox) const
{
	if (primBox.Intersects(quadTreeBox))
	{
		for (std::vector<GameObject*>::const_iterator it = this->gameobjs.begin(); it != this->gameobjs.end(); ++it)
		{
			if (primBox.Intersects((*it)->globalAABB))
				objects.push_back(*it);
		}
		for (int i = 0; i < 4; ++i)
			if (quTrChilds[i] != nullptr) quTrChilds[i]->Intersect(objects, primBox);
	}
}

void Quadtree::Intersect(std::map<float, GameObject*>& objects, const Ray & ray) const
{
	if (ray.Intersects(quadTreeBox))
	{
		float hit_near, hit_far;
		for (std::vector<GameObject*>::const_iterator it = this->gameobjs.begin(); it != this->gameobjs.end(); ++it)
		{
			if (ray.Intersects((*it)->globalAABB), hit_near, hit_far)
				objects[hit_near] = *it;
		}

		for (int i = 0; i < 4; ++i)
			if (quTrChilds[i] != nullptr) quTrChilds[i]->Intersect(objects, ray);
	}
}

void Quadtree::Subdivide(){

	float x = quadTreeBox.Size().x;
	float y = quadTreeBox.Size().y;
	float z = quadTreeBox.Size().z;

	float3 centerXZ = { quadTreeBox.HalfSize().x, y, quadTreeBox.HalfSize().z };

	//NW
	quTrChilds.push_back(new Quadtree(AABB(quadTreeBox.minPoint, quadTreeBox.minPoint + centerXZ), subdivisions + 1));

	//NE
	float3 neMin = quadTreeBox.minPoint + float3(quadTreeBox.HalfSize().x, 0.0f, 0.0f);
	quTrChilds.push_back(new Quadtree(AABB(neMin, neMin + centerXZ), subdivisions + 1));

	//SW
	float3 swMin = quadTreeBox.minPoint + float3(0.0f, 0.0f, quadTreeBox.HalfSize().z);
	quTrChilds.push_back(new Quadtree(AABB(swMin, swMin + centerXZ), subdivisions + 1));

	//SE
	quTrChilds.push_back(new Quadtree(AABB(quadTreeBox.maxPoint - centerXZ, quadTreeBox.maxPoint), subdivisions + 1));
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
