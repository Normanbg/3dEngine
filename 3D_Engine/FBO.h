#ifndef __FBO_H__
#define __FBO_H__

#include "Globals.h"
#include "Math.h"

//TUTORIAL USED: https://gamedev.stackexchange.com/questions/140693/how-can-i-render-an-opengl-scene-into-an-imgui-window

class FBO
{
public:
	FBO();
	~FBO();

	void Create(int width, int height);

	void BindFBO();
	void UnBindFBO();

public:
	uint texture;
	uint framebuffer;
	uint rbo;
	uint width;
	uint height;
};

#endif // !__FBO_H__
