#include "Quadtree.h"
#include "GameObject.h"
#include "ComponentMesh.h"

#include <vector>

Quadtree::Quadtree(AABB limits)
{
	quadTreeBox = limits;
}

Quadtree::~Quadtree()
{
}

void Quadtree::Clear(){
	if (!quTrChilds.empty()) {
		for (int i = 0; i < quTrChilds.size(); i++)
		{
			quTrChilds[i]->Clear();
			quTrChilds[i] = nullptr;
		}
		quTrChilds.clear();
	}
	for (int j = 0; j < gameobjs.size(); j++)
	{
		RELEASE(gameobjs[j]);
	}
	gameobjs.clear();
}

void Quadtree::Insert(GameObject * gameobject) {
	if (gameobject->GetComponentMesh() == nullptr)
	{
		OWN_LOG("%s require a mesh for the Bounding Box", gameobject->name.c_str());
		return;
	}
	if (quadTreeBox.Intersects(gameobject->GetComponentMesh()->boundingBox)) {
		if (!quTrChilds.empty()) {
			for (int i = 0; i < quTrChilds.size(); i++){
				quTrChilds[i]->Insert(gameobject);
			}
		}
		else {
			gameobjs.push_back(gameobject);
			if (QT_MAX_ELEMS < gameobjs.size())
			{
				if (quTrChilds.empty())
					Subdivide();

				for (int it = 0; it < gameobjs.size(); it++)
				{
					for (int i = 0; i < quTrChilds.size(); i++)
						quTrChilds[i]->Insert(gameobjs[it]);
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
			if (primitive.Intersects((*it)->GetComponentMesh()->boundingBox))
				objects.push_back(*it);
		}
		for (int i = 0; i < 4; ++i)
			if (quTrChilds[i] != nullptr) quTrChilds[i]->Intersect(objects, primitive);
	}
}

void Quadtree::Subdivide(){
	float3 centerXZ = { quadTreeBox.HalfSize().x, quadTreeBox.Size().y, quadTreeBox.HalfSize().z };
	
	//nw
	quTrChilds.push_back(new Quadtree(AABB(quadTreeBox.minPoint, quadTreeBox.minPoint + centerXZ)));

	//ne
	float3 neMin = quadTreeBox.minPoint + float3(centerXZ.x, 0.f, 0.f);
	quTrChilds.push_back(new Quadtree(AABB(neMin, neMin + centerXZ)));

	//sw 
	float3 swMin = quadTreeBox.minPoint + float3(0.f, 0.f, centerXZ.z);
	quTrChilds.push_back(new Quadtree(AABB(swMin, swMin + centerXZ)));

	//se
	quTrChilds.push_back(new Quadtree(AABB(float3(centerXZ.x, 0.f, centerXZ.z), quadTreeBox.maxPoint)));
	
}

void Quadtree::DebugDraw(){
	for (auto it : quTrChilds)
	{
		it->DebugDraw();
	}

	float3 corners[8];
	quadTreeBox.GetCornerPoints(corners);
	DebugDrawBox(corners, Red, 5.f);
}
