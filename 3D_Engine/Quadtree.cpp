#include "Quadtree.h"
#include "GameObject.h"

#include <vector>



Quadtree::Quadtree()
{
}


Quadtree::~Quadtree()
{
}

void Quadtree::Insert(GameObject * gameobject){
	if (gameobject->GetComponentMesh() == nullptr)
	{
		OWN_LOG("%s require a mesh for the Bounding Box", gameobject->name.c_str());
		return;
	}
	if (quadTreeBox.Contains(gameobject->GetComponentMesh()->boundingBox)) {
		if (gameobjs.empty())
			gameobjs.push_back(gameobject);
		//else
		//	//SPLIT
	}
}
