#ifndef  __PRIMITIVES_H__
#define __PRIMITIVES_H__

class GameObject;

class Primitives
{

public:
	Primitives();
	~Primitives();

	GameObject* CreateCube();
	GameObject* CreateSphere();
	
};


#endif // ! __PRIMITIVES_H__

