#ifndef __QUADTREE_H__
#define __QUADTREE_H__
#include "Math.h"

class GameObject;
class vector;

class Quadtree
{
public:
	Quadtree();
	~Quadtree();
	void Create(AABB limits);
	void Clear();
	void Insert(GameObject* gameobject);
	void Remove(GameObject* gameobject);
	bool Intersect(std::vector<GameObject*>& gameobjects, const AABB& bBox);

private:
	AABB quadTreeBox;
	std::vector<GameObject*> gameobjs;
	std::vector<Quadtree*> quTrChilds;
};
#endif // !__QUADTREE_H__
