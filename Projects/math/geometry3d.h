#pragma once

#include "linalg.h"

namespace mth
{
	struct Line3D
	{
		float3 point;
		float3 dir;

		Line3D() = default;
		Line3D(float3 point, float3 dir) :
			point(point), dir(dir) {}
	};

	float DistanceLineDot(Line3D line, float3 dot);
	float DistanceLineDotSquare(Line3D line, float3 dot);
	float DistanceLineDot(float3 linePoint, float3 lineDir, float3 dot);
	float DistanceLineDotSquare(float3 linePoint, float3 lineDir, float3 dot);

	float3x3 NormalToRotMat(float3 n);

	void TwoLinesMinDistPoints(Line3D l1, Line3D l2, float3& out1, float3& out2);
	void TwoLinesMinDistPoints(float3 p1, float3 v1, float3 p2, float3 v2, float3& out1, float3& out2);

	float3 LMSLineIntersection(Line3D lines[], int count);
	float3 LMSLineIntersection(float3 linePoints[], float3 lineDirs[], int count);

	float Random(float min = 0.0f, float max = 1.0f);
}