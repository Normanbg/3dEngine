#ifndef __MATH_H__
#define __MATH_H__

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include <math.h>
#include "Color.h"


void DebugDrawBox(float3* vertices, Color color = Green, float lineWidth = 1.5f);
void DebugDrawLine(const LineSegment line, const float4x4 & transform = float4x4::identity, Color color = Cyan, float lineWidth = 1.5f);


#endif // __MATH_H__