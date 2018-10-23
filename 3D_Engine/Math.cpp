#include "Math.h"

const float2 VecFunctions::rotatef2(const float2 &u, float angle)
{

	angle = angle / 180.0f * math::pi;

	float c = cos(angle), s = sin(angle);

	return float2(u.x * c - u.y * s, u.x * s + u.y * c);
}


const float3 VecFunctions::rotatef3(const float3 &u, float angle, const float3 &v)
{
	return *(float3*)&(rotatef4x4(angle, v) * float4(u, 1.0f));
}

const float4x4 VecFunctions::rotatef4x4(float angle, const float3 &u)
{
	float4x4 Rotate;

	angle = angle / 180.0f * math::pi;

	float3 v = u;
	v.Normalize();

	float c = 1.0f - cos(angle), s = sin(angle);


	Rotate.RotateAxisAngle(v, angle);

	return Rotate;
}

const float4x4 VecFunctions::perspective(float fovy, float aspect, float n, float f)
{
	float coty = 1.0f / tan(fovy * (float)math::pi / 360.0f);
	float4x4 mat;

	mat.At(0, 0) = coty / aspect;
	mat.At(1, 1) = coty;
	mat.At(2, 2) = (n + f) / (n - f);
	mat.At(3, 2) = -1.0f;
	mat.At(2, 3) = 2.0f * n * f / (n - f);
	mat.At(3, 3) = 0.0f;
	return mat;
}

const float4x4 VecFunctions::ortho(float left, float right, float bottom, float top, float n, float f)
{
	float4x4 Ortho;

	Ortho.At(0, 0) = 2.0f / (right - left);
	Ortho.At(1, 1) = 2.0f / (top - bottom);
	Ortho.At(2, 2) = -2.0f / (f - n);
	Ortho.At(0, 3) = -(right + left) / (right - left);
	Ortho.At(1, 3) = -(top + bottom) / (top - bottom);
	Ortho.At(2, 3) = -(f + n) / (f - n);

	return Ortho;
}