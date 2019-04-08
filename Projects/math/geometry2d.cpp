#include "geometry2d.h"

namespace mth
{
	int Quadratic(float a, float b, float c, float& s1, float& s2)
	{
		float dis = b * b - 4.0f*a*c;
		if (dis < 0.0f)
			return 0;
		if (dis == 0.0f)
		{
			s1 = s2 = -b / (2.0f*a);
			return 1;
		}
		a = 0.5f / a;
		b *= -a;
		dis = sqrtf(dis)*a;
		s1 = b + dis;
		s2 = b - dis;
		return 2;
	}

	Point2D::Point2D() :p() {}
	Point2D::Point2D(float2 point) : p(point) {}

	Line2D::Line2D() : p(), v() {}
	Line2D::Line2D(float2 point, float angle) :
		p(point),
		v(sinf(angle), cosf(angle)) {}
	Line2D::Line2D(float2 point, float2 direction) :
		p(point), v(direction) {}
	Line2D Line2D::From2Points(float2 p1, float2 p2)
	{
		return Line2D(p1, p2 - p1);
	}
	float Line2D::Angle()
	{
		return atan2f(v.y, v.x);
	}

	float2 Line2D::Intersection(Line2D line)
	{
		return Intersection2Lines(p, v, line.p, line.v);
	}

	Circle2D::Circle2D() :c(), r(0.0f) {}
	Circle2D::Circle2D(float2 center, float radius) : c(center), r(radius) {}
	Circle2D Circle2D::SameAngleCircle(mth::float2 p1, mth::float2 p2, float a)
	{
		mth::float2 pm = (p1 + p2)*0.5f;
		float angle = mth::PI*0.5f + atanf((p2.y - p1.y) / (p2.x - p1.x));
		mth::float2 d0(cosf(angle), sinf(angle));
		mth::float2 d1(cosf(angle + a), sinf(angle + a));
		mth::float2 c1 = Intersection2Lines(pm, d0, p1, d1);
		//mth::float2 c2 = Intersection2Lines(pm, d0, p2, d1);
		return Circle2D(c1, (c1 - p1).Length());

	}
	bool Circle2D::IsOnArea(float2 p)
	{
		return (c - p).LengthSquare() <= r * r;
	}
	bool Circle2D::IsOnPerimeter(float2 p, float eps)
	{
		return IsPointOnCircle(p, c, r, eps);
	}
	int Circle2D::Intersection(Line2D line, float2 out[2])
	{
		return IntersectionCircleLine(c, r, line.p, line.v, out);
	}
	int Circle2D::Intersection(Circle2D circle, float2 out[2])
	{
		return Intersection2Circles(c, r, circle.c, circle.r, out);
	}

	float2 Intersection2Lines(float2 p1, float2 v1, float2 p2, float2 v2)
	{
		float2 x1 = p1, x2 = p1 + v1, y1 = p2, y2 = p2 + v2;
		float2 r;
		float f1, f2;
		f1 = (x1.x*x2.y - x1.y*x2.x)*(y1.x - y2.x) - (x1.x - x2.x)*(y1.x*y2.y - y1.y*y2.x);
		f2 = (x1.x - x2.x)*(y1.y - y2.y) - (y1.x - y2.x)*(x1.y - x2.y);
		r.x = f1 / f2;
		f1 = (x1.x*x2.y - x1.y*x2.x)*(y1.y - y2.y) - (x1.y - x2.y)*(y1.x*y2.y - y1.y*y2.x);
		r.y = f1 / f2;
		return r;
	}

	int Intersection2Circles(float2 p1, float r1, float2 p2, float r2, float2 out[2])
	{
		float2 p = p2 - p1;
		float f = (p.x*p.x + p.y*p.y + r1 * r1 - r2 * r2) / (2.0f * p.y);
		float x1, x2;
		int ret = Quadratic((p.x*p.x) / (p.y*p.y) + 1.0f, -2.0f*f * p.x / p.y, f * f - r1 * r1, x1, x2);
		if (ret == 0) return ret;

		float2 s1, s2;
		s1.x = x1;
		s1.y = sqrtf(r1*r1 - s1.x*s1.x);
		s2.x = s1.x;
		s2.y = -s1.y;
		out[0] = p1 + ((fabsf((s1 - p).LengthSquare() - r2 * r2) < fabsf((s2 - p).LengthSquare() - r2 * r2)) ? s1 : s2);
		if (ret == 1) return ret;

		s1.x = x2;
		s1.y = sqrtf(r1*r1 - s1.x*s1.x);
		s2.x = s1.x;
		s2.y = -s1.y;
		out[1] = p1 + ((fabsf((s1 - p).LengthSquare() - r2 * r2) < fabsf((s2 - p).LengthSquare() - r2 * r2)) ? s1 : s2);
		return ret;
	}

	int IntersectionCircleLine(float2 c, float r, float2 p, float2 v, float2 out[2])
	{
		float sols[2];
		float2 tmp = p - c;
		int ret = Quadratic(v.LengthSquare(), 2.0f*tmp.Dot(v), tmp.LengthSquare() - r * r, sols[0], sols[1]);
		if (ret == 0)
			return 0;
		out[0] = p + sols[0] * v;
		out[1] = p + sols[1] * v;
		return ret;
	}

	bool IsPointOnCircle(float2 p, float2 c, float r, float eps)
	{
		return fabsf((p - c).LengthSquare() - r * r) < eps;
	}
}