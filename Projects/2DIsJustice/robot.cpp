#include "robot.h"

bool Robot::GlobalToScreen(mth::float2 p, float& out)
{
	p = mth::float2x2::Rotation(m_angle)*(p - m_roboPos);
	if (p.y <= 0.0f)
		return false;
	float s = p.x / (-p.y * tanf(m_fov));
	if (fabsf(s) > 1.0f)
		return false;
	out = s;
	return true;
}

float Robot::GlobalToAngle(mth::float2 p)
{
	return atan2f(m_roboPos.y - p.y, m_roboPos.x - p.x) - m_angle;
}

mth::float2 Robot::ScreenToGlobal(float s, float dist)
{
	return ScreenToRelative(s, dist) + m_roboPos;
}

mth::float2 Robot::ScreenToRelative(float s, float dist)
{
	mth::float2 p;
	p.x = -s * dist*tanf(m_fov);
	p.y = dist;
	return p * mth::float2x2::Rotation(m_angle);
}

void Robot::UpdateAngleLookAtCursor()
{
	m_angle = atan2f(m_mousePos.x - m_roboPos.x, m_mousePos.y - m_roboPos.y);
}

Robot::Robot()
{
	m_APressed = false;
	m_SPressed = false;
	m_DPressed = false;
	m_WPressed = false;
	m_speed = 400.0f;
	m_roboRadius = 42.0f;
	m_roboPos = mth::float2(100.0f, 100.0f);
	m_roboVisionHalfSize = 50.0f;
	m_fov = 0.6f;
	m_lineLen = 2.5f*m_roboRadius;
	m_angle = 0.0f;
}

void Robot::UpdateSight(std::vector<mth::float2>& points)
{
	m_sight.clear();
	for (auto p : points)
	{
		float s;
		if (GlobalToScreen(p, s))
			m_sight.push_back(s);
	}
}

void Robot::Update(float delta)
{
	if (m_APressed)
		m_roboPos.x -= m_speed * delta;
	if (m_DPressed)
		m_roboPos.x += m_speed * delta;
	if (m_SPressed)
		m_roboPos.y += m_speed * delta;
	if (m_WPressed)
		m_roboPos.y -= m_speed * delta;
	UpdateAngleLookAtCursor();
}

void Robot::Draw(HDC hdc)
{
	HBRUSH hb = CreateSolidBrush(RGB(140, 160, 190));
	HPEN hp = CreatePen(PS_SOLID, 3, RGB(50, 60, 80));
	HPEN thin = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));

	SelectObject(hdc, hp);
	SelectObject(hdc, hb);
	Ellipse(hdc, (int)(m_roboPos.x - m_roboRadius), (int)(m_roboPos.y - m_roboRadius), (int)(m_roboPos.x + m_roboRadius), (int)(m_roboPos.y + m_roboRadius));

	MoveToEx(hdc, (int)(m_roboPos.x + sinf(m_angle + m_fov)*m_lineLen), (int)(m_roboPos.y + cosf(m_angle + m_fov)*m_lineLen), NULL);
	LineTo(hdc, (int)(m_roboPos.x), (int)(m_roboPos.y));
	LineTo(hdc, (int)(m_roboPos.x + sinf(m_angle - m_fov)*m_lineLen), (int)(m_roboPos.y + cosf(m_angle - m_fov)*m_lineLen));

	DeleteObject(hb);
	hb = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hb);
	Rectangle(hdc, (int)(m_roboPos.x - m_roboVisionHalfSize), (int)(m_roboPos.y - m_roboRadius - 10.0f), (int)(m_roboPos.x + m_roboVisionHalfSize), (int)(m_roboPos.y - m_roboRadius));

	for (auto s : m_sight)
	{
		SelectObject(hdc, hp);
		MoveToEx(hdc, (int)(m_roboPos.x + m_roboVisionHalfSize * s), (int)(m_roboPos.y - m_roboRadius - 10.0f), NULL);
		LineTo(hdc, (int)(m_roboPos.x + m_roboVisionHalfSize * s), (int)(m_roboPos.y - m_roboRadius));

		SelectObject(hdc, thin);
		MoveToEx(hdc, (int)m_roboPos.x, (int)m_roboPos.y, NULL);
		mth::float2 p = ScreenToGlobal(s, 3000.0f);
		LineTo(hdc, (int)p.x, (int)p.y);
	}

	DeleteObject(thin);
	DeleteObject(hp);
	DeleteObject(hb);
}

void Robot::MouseMove(int x, int y)
{
	m_mousePos.x = (float)x;
	m_mousePos.y = (float)y;
}
void Robot::KeyDown(WPARAM key)
{
	switch (key)
	{
	case 'A':
		m_APressed = true;
		break;
	case 'S':
		m_SPressed = true;
		break;
	case 'D':
		m_DPressed = true;
		break;
	case 'W':
		m_WPressed = true;
		break;
	}
}
void Robot::KeyUp(WPARAM key)
{
	switch (key)
	{
	case 'A':
		m_APressed = false;
		break;
	case 'S':
		m_SPressed = false;
		break;
	case 'D':
		m_DPressed = false;
		break;
	case 'W':
		m_WPressed = false;
		break;
	}
}

void Robot::CopySight(Robot& other)
{
	m_sight = other.m_sight;
}

float Robot::getSightAngle(unsigned i)
{
	if (i >= m_sight.size())
		return 0.0f;
	mth::float2 p;
	p.x = -m_sight[i] * tanf(m_fov);
	p.y = 1.0f;
	return atan2f(p.y, p.x);
}

mth::float2 Robot::getEstimationGlobal(unsigned i, float dist)
{
	if (i >= m_sight.size())
		return mth::float2();
	return ScreenToGlobal(m_sight[i], dist);
}

mth::float2 Robot::getEstimationRelative(unsigned i, float dist)
{
	if (i >= m_sight.size())
		return mth::float2();
	return ScreenToRelative(m_sight[i], dist);
}
