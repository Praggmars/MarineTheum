#include "geometry3d.h"

namespace mth
{
	float DistanceLineDot(Line3D line, float3 dot)
	{
		return DistanceLineDot(line.point, line.dir, dot);
	}
	float DistanceLineDotSquare(Line3D line, float3 dot)
	{
		return DistanceLineDotSquare(line.point, line.dir, dot);
	}
	float DistanceLineDot(float3 linePoint, float3 lineDir, float3 dot)
	{
		return sqrtf(DistanceLineDotSquare(linePoint, lineDir, dot));
	}
	float DistanceLineDotSquare(float3 linePoint, float3 lineDir, float3 dot)
	{
		return lineDir.Cross(dot - linePoint).LengthSquare() / lineDir.LengthSquare();
	}
	float3x3 NormalToRotMat(float3 n)
	{
		if (n.z == 1.0f)
			return float3x3::Identity();
		float3 u1, u2, u3;
		u3 = n;
		u1 = u3.Cross(mth::float3(0.0f, 0.0f, 1.0f)).Normalized();
		u2 = u1.Cross(u3).Normalized();
		float3x3 m(
			u1.x, u1.y, u1.z,
			u2.x, u2.y, u2.z,
			u3.x, u3.y, u3.z);
		return m;
	}
	void TwoLinesMinDistPoints(Line3D l1, Line3D l2, float3& out1, float3& out2)
	{
		TwoLinesMinDistPoints(l1.point, l1.dir, l2.point, l2.dir, out1, out2);
	}
	void TwoLinesMinDistPoints(float3 p1, float3 v1, float3 p2, float3 v2, float3& out1, float3& out2)
	{
		float3x3 rot = NormalToRotMat(v1.Cross(v2).Normalized());
		float3 p1p = rot * p1;
		float3 p2p = rot * p2;
		float3 v1p = rot * v1;
		float3 v2p = rot * v2;
		float2 t = float2x2(v1p.x, -v2p.x, v1p.y, -v2p.y).Inverse()*float2(p2p.x - p1p.x, p2p.y - p1p.y);
		out1 = p1 + v1 * t.x;
		out2 = p2 + v2 * t.y;
	}
	float3 LMSLineIntersection(Line3D lines[], int count)
	{
		float3x3 mat;
		float3 vec;
		for (int i = 0; i < count; i++)
		{
			float3 p = lines[i].point;
			float3 v = lines[i].dir;
			float oneovervls = 1.0f / v.LengthSquare();
			float3x3 tmp = float3x3::Identity() - float3x3(
				v.x*v.x, v.x*v.y, v.x*v.z,
				v.y*v.x, v.y*v.y, v.y*v.z,
				v.z*v.x, v.z*v.y, v.z*v.z
			)*oneovervls;
			mat += tmp;
			vec += tmp * p;
		}
		return mat.Inverse()*vec;
	}
	float3 LMSLineIntersection(float3 linePoints[], float3 lineDirs[], int count)
	{
		float3x3 mat;
		float3 vec;
		for (int i = 0; i < count; i++)
		{
			float3 p = linePoints[i];
			float3 v = lineDirs[i];
			float oneovervls = 1.0f / v.LengthSquare();
			float3x3 tmp = float3x3::Identity() - float3x3(
				v.x*v.x, v.x*v.y, v.x*v.z,
				v.y*v.x, v.y*v.y, v.y*v.z,
				v.z*v.x, v.z*v.y, v.z*v.z
			)*oneovervls;
			mat += tmp;
			vec += tmp * p;
		}
		return mat.Inverse()*vec;
	}

	float Random(float min, float max)
	{
		return (float)rand() / (float)RAND_MAX*(max - min) + min;
	}
}