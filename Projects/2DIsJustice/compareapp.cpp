#include "compareapp.h"
#include "genmath.h"
#include "drawing.h"

void CompareApp::DrawPoints(HDC hdc)
{
	HBRUSH hb = CreateSolidBrush(RGB(230, 255, 240));
	HPEN hp = CreatePen(PS_SOLID, 3, RGB(100, 50, 60));
	SelectObject(hdc, hp);
	SelectObject(hdc, hb);

	for (auto p : m_points)
		DrawSphere(hdc, p, m_pointRadius);

	DeleteObject(hp);
	DeleteObject(hb);
}

void CompareApp::DrawEstimatedPoints(HDC hdc)
{
	HBRUSH hb = CreateSolidBrush(RGB(255, 230, 210));
	HPEN hp = CreatePen(PS_SOLID, 3, RGB(200, 50, 60));
	SelectObject(hdc, hp);
	SelectObject(hdc, hb);

	for (unsigned i = 0; i < m_points.size(); i++)
	{
		mth::float2 p = Intersection2Lines(
			m_robo1.getPosition(), m_robo1.getEstimationRelative(i, 1.0f),
			m_robo2.getPosition(), m_robo2.getEstimationRelative(i, 1.0f));
		DrawSphere(hdc, p, m_pointRadius);
	}

	DeleteObject(hp);
	DeleteObject(hb);
}

bool CompareApp::isRoboClicked(Robot& robo, mth::float2 p)
{
	return (p - robo.getPosition()).LengthSquare() < robo.getRoboRadius()*robo.getRoboRadius();
}

void CompareApp::Draw3PointsSameAngles(HDC hdc)
{
	m_robo1.UpdateSight(m_points);

	mth::float2 p1(400.0f, 470.0f);
	mth::float2 p2(600.0f, 540.0f);
	mth::float2 p3(560.0f, 420.0f);
	m_points.clear();
	m_points.push_back(p1);
	m_points.push_back(p2);
	m_points.push_back(p3);
	HPEN hp;
	hp = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	SelectObject(hdc, hp);
	SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
	float a1 = m_robo1.GlobalToAngle(p1);
	float a2 = m_robo1.GlobalToAngle(p2);
	float a3 = m_robo1.GlobalToAngle(p3);

	hp = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	SelectObject(hdc, hp);
	DrawSameAngleSpheres(hdc, p1, p2, a1 - a2);
	DeleteObject(hp);

	hp = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	SelectObject(hdc, hp);
	DrawSameAngleSpheres(hdc, p2, p3, a2 - a3);
	DeleteObject(hp);

	hp = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	SelectObject(hdc, hp);
	DrawSameAngleSpheres(hdc, p3, p1, a3 - a1);
	DeleteObject(hp);

	DrawPoints(hdc);

	m_robo1.Draw(hdc);
}

void CompareApp::Paint(HDC hdc, int w, int h)
{
	PatBlt(hdc, 0, 0, w, h, WHITENESS);

	//m_robo1.UpdateSight(m_points);
	//m_robo2.UpdateSight(m_points);
	DrawEstimatedPoints(hdc);
	//Draw3PointsSameAngles(hdc);
	m_robo1.Draw(hdc);
	m_robo2.Draw(hdc);

	//DrawPoints(hdc);
}

void CompareApp::MouseMove(int x, int y, int dx, int dy)
{
	if (m_selectedRobo)
	{
		m_selectedRobo->setPosition(mth::float2((float)x, (float)y) - m_grabOffset);
		AskRedraw();
	}
}
void CompareApp::LBtnDown(int x, int y)
{
	mth::float2 p((float)x, (float)y);
	if (isRoboClicked(m_robo1, p))
	{
		m_grabOffset = p - m_robo1.getPosition();
		m_selectedRobo = &m_robo1;
	}
	else if (isRoboClicked(m_robo2, p))
	{
		m_grabOffset = p - m_robo2.getPosition();
		m_selectedRobo = &m_robo2;
	}
}
void CompareApp::LBtnUp(int x, int y)
{
	m_selectedRobo = nullptr;
}

void CompareApp::MouseWheel(bool up)
{
	if (m_selectedRobo)
	{
		float delta = up ? 0.2f : -0.2f;
		m_selectedRobo->setAngle(m_selectedRobo->getAngle() + delta);
		AskRedraw();
	}
}

CompareApp::CompareApp(HWND hwnd) :WindowClass(hwnd)
{
	m_pointRadius = 10.0f;

	m_points.push_back(mth::float2(200.0f, 200.0f));
	m_points.push_back(mth::float2(300.0f, 200.0f));
	//m_points.push_back(mth::float2(200.0f, 300.0f));
	m_points.push_back(mth::float2(300.0f, 300.0f));

	m_selectedRobo = nullptr;
	m_robo1.setPosition(mth::float2(220.0f, 530.0f));
	m_robo1.setAngle(mth::PI);
	m_robo1.UpdateSight(m_points);
	m_robo2.setPosition(mth::float2(310.0f, 480.0f));
	m_robo2.setAngle(3.4f);
	m_robo2.UpdateSight(m_points);
}

CompareApp::~CompareApp() {}
