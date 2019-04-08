#pragma once

#include "linalg.h"

namespace mth
{
	class Point2D
	{
	public:
		float2 p;

	public:
		Point2D();
		Point2D(float2 point);
	};

	class Line2D
	{
	public:
		float2 p;
		float2 v;

	public:
		Line2D();
		Line2D(float2 point, float angle);
		Line2D(float2 point, float2 direction);
		static Line2D From2Points(float2 p1, float2 p2);

		float Angle();
		float2 Intersection(Line2D line);
	};

	class Circle2D
	{
	public:
		float2 c;
		float r;

	public:
		Circle2D();
		Circle2D(float2 center, float radius);
		static Circle2D SameAngleCircle(mth::float2 p1, mth::float2 p2, float a);

		bool IsOnArea(float2 p);
		bool IsOnPerimeter(float2 p, float eps = EPS);
		int Intersection(Line2D line, float2 out[2]);
		int Intersection(Circle2D circle, float2 out[2]);
	};

	int Quadratic(float a, float b, float c, float& s1, float& s2);
	float2 Intersection2Lines(float2 p1, float2 v1, float2 p2, float2 v2);
	int Intersection2Circles(float2 c1, float r1, float2 c2, float r2, float2 out[2]);
	int IntersectionCircleLine(float2 c, float r, float2 p, float2 v, float2 out[2]);
	bool IsPointOnCircle(float2 p, float2 c, float r, float eps = EPS);

	Circle2D SameAngleCircle(mth::float2 p1, mth::float2 p2, float a);
}