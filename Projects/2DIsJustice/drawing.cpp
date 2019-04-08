#include "drawing.h"

void DrawSphere(HDC hdc, mth::float2 p, float r)
{
	Ellipse(hdc, (int)(p.x - r), (int)(p.y - r), (int)(p.x + r), (int)(p.y + r));
}

void DrawEndlessLine(HDC hdc, mth::float2 p, float a)
{
	mth::float2 d(cosf(a), sinf(a));
	MoveToEx(hdc, (int)(p.x - d.x * 5000.0f), (int)(p.y - d.y * 5000.0f), NULL);
	LineTo(hdc, (int)(p.x + d.x * 5000.0f), (int)(p.y + d.y * 5000.0f));
}

void DrawSameAngleSpheres(HDC hdc, mth::float2 p1, mth::float2 p2, float a)
{
	mth::Circle2D c = mth::Circle2D::SameAngleCircle(p1, p2, a);
	DrawSphere(hdc, c.c, c.r);
}
