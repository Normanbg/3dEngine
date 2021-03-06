#ifndef __QUADTREE_H__
#define __QUADTREE_H__
#include "Globals.h"
#include "Math.h"

#include <map>
#include <list>

constexpr auto QT_MAX_ELEMS = 1;
constexpr auto MAX_SUBDIVISIONS = 10;

class GameObject;
class vector;
//class list;

class Quadtree
{
public:
	Quadtree(AABB limits, uint subdivision);
	~Quadtree();
	

	void Clear();
	void Insert(GameObject* gameobject);

	void Intersect(std::vector<GameObject*>& gameobjects, const AABB& bBox) const;
	void Intersect(std::map<float, GameObject*>& objects, const Ray& bBox) const;

	void Intersects(std::list<uuid>& inter_list, Frustum frustum);

	void Subdivide();

	void DebugDraw();
public:
	
	std::vector<GameObject*> gameobjs;
	std::vector<Quadtree*> quTrChilds;
	AABB quadTreeBox;
 
private:
	uint subdivisions = 0;
};
#endif // !__QUADTREE_H__
