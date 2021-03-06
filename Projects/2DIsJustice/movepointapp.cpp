#include "movepointapp.h"
#include "drawing.h"

void MovePointApp::StartUpdateControl()
{
	SetTimer(m_hwnd, 1, 20, NULL);
	m_prevTime = std::chrono::steady_clock::now();
	m_roboUpdateControl = true;
}
void MovePointApp::StopUpdateControl()
{
	KillTimer(m_hwnd, 1);
	m_roboUpdateControl = false;
}

void MovePointApp::Update(float delta)
{
	m_inputRobo.Update(delta);
}

void MovePointApp::DrawRobots(HDC hdc)
{
	m_inputRobo.UpdateSight(m_inputPoints);
	m_inputRobo.Draw(hdc);

	m_estRobo.CopySight(m_inputRobo);
	EstimateRoboPose();
	m_estRobo.Draw(hdc);
}

void MovePointApp::DrawPoints(HDC hdc)
{
	HBRUSH hb = CreateSolidBrush(RGB(230, 255, 240));
	HPEN hp = CreatePen(PS_SOLID, 3, RGB(100, 50, 60));
	SelectObject(hdc, hp);
	SelectObject(hdc, hb);

	for (auto p : m_inputPoints)
		DrawSphere(hdc, p, m_pointRadius);

	DeleteObject(hp);
	DeleteObject(hb);

	hb = CreateSolidBrush(RGB(255, 240, 230));
	hp = CreatePen(PS_SOLID, 3, RGB(100, 50, 60));
	SelectObject(hdc, hp);
	SelectObject(hdc, hb);

	for (auto p : m_estPoints)
		DrawSphere(hdc, p, m_pointRadius);
	DeleteObject(hp);
	DeleteObject(hb);
}

void MovePointApp::EstimateRoboPose()
{
	mth::Circle2D c1 = mth::Circle2D::SameAngleCircle(m_estPoints[0], m_estPoints[1], m_estRobo.getSightAngle(0) - m_estRobo.getSightAngle(1));
	mth::Circle2D c2 = mth::Circle2D::SameAngleCircle(m_estPoints[1], m_estPoints[2], m_estRobo.getSightAngle(1) - m_estRobo.getSightAngle(2));
	mth::Circle2D c3 = mth::Circle2D::SameAngleCircle(m_estPoints[2], m_estPoints[0], m_estRobo.getSightAngle(2) - m_estRobo.getSightAngle(0));

	mth::float2 iscts[3];
	mth::float2 sols[2];

	c1.Intersection(c2, sols);
	iscts[0] = sols[(m_estPoints[1] - sols[0]).LengthSquare() > (m_estPoints[1] - sols[1]).LengthSquare() ? 0 : 1];
	c2.Intersection(c3, sols);
	iscts[1] = sols[(m_estPoints[2] - sols[0]).LengthSquare() > (m_estPoints[2] - sols[1]).LengthSquare() ? 0 : 1];
	c3.Intersection(c1, sols);
	iscts[2] = sols[(m_estPoints[0] - sols[0]).LengthSquare() > (m_estPoints[0] - sols[1]).LengthSquare() ? 0 : 1];

	m_estRobo.setPosition((iscts[0] + iscts[0] + iscts[0]) / 3.0f);
	mth::float2 p = m_estPoints[0] - m_estRobo.getPosition();
	float ga = atan2f(p.y, p.x);
	float ra = m_estRobo.getSightAngle(0);
	m_estRobo.setAngle(ra - ga);
}

MovePointApp::MovePointApp(HWND hwnd) :WindowClass(hwnd)
{
	m_pointRadius = 10.0f;
	m_selectedPoint = nullptr;
	m_selectedRobo = nullptr;

	m_inputPoints.push_back(mth::float2(300.0f, 250.0f));
	m_inputPoints.push_back(mth::float2(400.0f, 300.0f));
	m_inputPoints.push_back(mth::float2(390.0f, 400.0f));
	m_inputRobo.setPosition(mth::float2(340.0f, 600.0f));
	m_inputRobo.setAngle(mth::PI);

	m_estPoints.push_back(mth::float2(900.0f, 250.0f));
	m_estPoints.push_back(mth::float2(1000.0f, 300.0f));
	m_estPoints.push_back(mth::float2(990.0f, 400.0f));
	m_estRobo.setPosition(mth::float2(940.0f, 600.0f));

	m_roboUpdateControl = false;
	m_estRobo.setAngle(mth::PI);
}
MovePointApp::~MovePointApp()
{
	StopUpdateControl();
}

void MovePointApp::Paint(HDC hdc, int w, int h)
{
	PatBlt(hdc, 0, 0, w, h, WHITENESS);
	DrawRobots(hdc);
	DrawPoints(hdc);
}

void MovePointApp::MouseMove(int x, int y, int dx, int dy)
{
	if (m_selectedPoint)
	{
		*m_selectedPoint = mth::float2((float)x, (float)y) - m_grabOffset;
		AskRedraw();
	}
	if (m_selectedRobo)
	{
		m_selectedRobo->setPosition(mth::float2((float)x, (float)y) - m_grabOffset);
		AskRedraw();
	}
	if (m_roboUpdateControl)
	{
		m_inputRobo.MouseMove(x, y);
	}
}

void MovePointApp::KeyDown(WPARAM wparam)
{
	if (wparam == VK_SPACE)
		m_roboUpdateControl ? StopUpdateControl() : StartUpdateControl();
	if (m_roboUpdateControl)
		m_inputRobo.KeyDown(wparam);
}

void MovePointApp::KeyUp(WPARAM wparam)
{
	if (m_roboUpdateControl)
		m_inputRobo.KeyUp(wparam);
}

void MovePointApp::LBtnDown(int x, int y)
{
	mth::float2 m((float)x, (float)y);
	for (auto& p : m_inputPoints)
	{
		if ((p - m).LengthSquare() < m_pointRadius*m_pointRadius)
		{
			m_selectedPoint = &p;
			m_grabOffset = m - p;
			return;
		}
	}
	for (auto& p : m_estPoints)
	{
		if ((p - m).LengthSquare() < m_pointRadius*m_pointRadius)
		{
			m_selectedPoint = &p;
			m_grabOffset = m - p;
			return;
		}
	}
	if ((m_inputRobo.getPosition() - m).LengthSquare() < m_inputRobo.getRoboRadius()*m_inputRobo.getRoboRadius())
	{
		m_selectedRobo = &m_inputRobo;
		m_grabOffset = m - m_inputRobo.getPosition();
		return;
	}
}

void MovePointApp::LBtnUp(int x, int y)
{
	m_selectedPoint = nullptr;
	m_selectedRobo = nullptr;
}

void MovePointApp::MouseWheel(bool up)
{
	if (!m_roboUpdateControl && m_selectedRobo)
	{
		float delta = up ? 0.2f : -0.2f;
		m_selectedRobo->setAngle(m_selectedRobo->getAngle() + delta);
		AskRedraw();
	}
}

void MovePointApp::Timer(WPARAM wparam)
{
	if (wparam == 1)
	{
		decltype(m_prevTime) currTime = std::chrono::steady_clock::now();
		Update(std::chrono::duration<float>(currTime - m_prevTime).count());
		m_prevTime = currTime;
		AskRedraw();
	}
}
