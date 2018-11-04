#include "Quadtree.h"
#include "GameObject.h"

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

				for (auto it : gameobjs)
				{
					for (int i = 0; i < quTrChilds.size(); i++)
						quTrChilds[i]->Insert(it);
				}

				gameobjs.clear();
			}
			
		}
	}
}

void Quadtree::Subdivide(){
	float3 half = { quadTreeBox.HalfSize().x, quadTreeBox.Size().y, quadTreeBox.HalfSize().z };
	
	//nw
	quTrChilds.push_back(new Quadtree(AABB(quadTreeBox.minPoint, quadTreeBox.minPoint + half)));

	//ne
	float3 neMin = quadTreeBox.minPoint + float3(half.x / 2, 0.f, 0.f);
	quTrChilds.push_back(new Quadtree(AABB(neMin, neMin + half)));

	//sw 
	float3 swMin = quadTreeBox.minPoint + float3(0.f, 0.f, half.z / 2);
	quTrChilds.push_back(new Quadtree(AABB(swMin, swMin + half)));

	//se
	quTrChilds.push_back(new Quadtree(AABB(quadTreeBox.maxPoint - half, quadTreeBox.maxPoint)));
	
}

void Quadtree::DebugDraw(){
	float3 corners[8];
	quadTreeBox.GetCornerPoints(corners);
	DebugDrawBox(corners, Red);
}
