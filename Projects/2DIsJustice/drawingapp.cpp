#include "drawingapp.h"

DrawingApp::DrawingApp(HWND hwnd) :
	WindowClass(hwnd),
	m_circle1(mth::float2(250.0f, 200.0f), 130.0f),
	m_circle2(mth::float2(400.0f, 320.0f), 170.0f),
	m_line1(mth::float2(400.0f, 0.0f), mth::float2(1.0f, 2.0f)),
	m_line2(mth::float2(0.0f, 300.0f), mth::float2(10.0f, -1.0f))
{}

void DrawingApp::Paint(HDC hdc, int w, int h)
{
	PatBlt(hdc, 0, 0, w, h, WHITENESS);
	SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
	DrawEndlessLine(hdc, m_line1.p, m_line1.Angle());
	DrawEndlessLine(hdc, m_line2.p, m_line2.Angle());
	DrawSphere(hdc, m_circle1.c, m_circle1.r);
	DrawSphere(hdc, m_circle2.c, m_circle2.r);

	mth::float2 s[2];
	float r = 10.0f;
	int count;

	count = m_circle1.Intersection(m_circle2, s);
	for (int i = 0; i < count; i++)
		DrawSphere(hdc, s[i], r);
	count = m_circle1.Intersection(m_line1, s);
	for (int i = 0; i < count; i++)
		DrawSphere(hdc, s[i], r);
	count = m_circle1.Intersection(m_line2, s);
	for (int i = 0; i < count; i++)
		DrawSphere(hdc, s[i], r);
	count = m_circle2.Intersection(m_line1, s);
	for (int i = 0; i < count; i++)
		DrawSphere(hdc, s[i], r);
	count = m_circle2.Intersection(m_line2, s);
	for (int i = 0; i < count; i++)
		DrawSphere(hdc, s[i], r);
	DrawSphere(hdc, m_line1.Intersection(m_line2), r);
}

void DrawingApp::MouseMove(int x, int y, int dx, int dy)
{
}

void DrawingApp::LBtnDown(int x, int y)
{
}

void DrawingApp::LBtnUp(int x, int y)
{
}
