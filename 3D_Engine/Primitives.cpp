#include "Primitives.h"
#include "GameObject.h"

Primitives::Primitives()
{
}


Primitives::~Primitives()
{
}

GameObject * Primitives::CreateCube()
{
	/*glGenBuffers(1, (GLuint*) &(buffBox2ID));
	glBindBuffer(GL_ARRAY_BUFFER, buffBox2ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*box2.size() * 3, &box2[0], GL_STATIC_DRAW);
	boxIndices = { 0,1,2 , 1,3,2 , 3,1,5 , 5,7,3 , 7,5,4 , 6,7,4 , 6,4,0, 0,2,6  , 6,2,3 , 6,3,7 , 0,4,5 , 0,5,1 };*/
}

GameObject * Primitives::CreateSphere()
{
	return nullptr;
}
