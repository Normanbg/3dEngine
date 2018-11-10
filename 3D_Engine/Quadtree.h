#ifndef __QUADTREE_H__
#define __QUADTREE_H__
#include "Globals.h"
#include "Math.h"

constexpr auto QT_MAX_ELEMS = 1;
constexpr auto MAX_SUBDIVISIONS = 4;

class GameObject;
class vector;

class Quadtree
{
public:
	Quadtree(AABB limits, uint subdivision);
	~Quadtree();
	

	void Clear();
	void Insert(GameObject* gameobject);
	void Remove(GameObject* gameobject);
	void Intersect(std::vector<GameObject*>& gameobjects, const AABB& bBox) const;

	void Subdivide();

	void AddGOtoQuadtree(GameObject * go);
	void Resize(GameObject * go);

	void DebugDraw();

private:
	uint subdivisions = 0;
	AABB quadTreeBox;
	std::vector<GameObject*> gameobjs;
	std::vector<Quadtree*> quTrChilds;
};
#endif // !__QUADTREE_H__
