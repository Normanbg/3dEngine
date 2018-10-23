#ifndef __MATH_H__
#define __MATH_H__

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include <math.h>


namespace VecFunctions
{

	const float2 rotatef2(const float2 &u, float angle);
	const float3 rotatef3(const float3 &u, float angle, const float3 &v);
	const float4x4 rotatef4x4(float angle, const float3 &u);
	const float4x4 perspective(float fovy, float aspect, float n, float f);
	const float4x4 ortho(float left, float right, float bottom, float top, float n, float f);

};

#endif // __MATH_H__