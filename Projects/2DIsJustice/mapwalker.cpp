#include "mapwalker.h"
#include "drawing.h"
#include <string>

void MapWalker::RandomPoints(int count, int w, int h)
{
	for (int i = 0; i < count; i++)
		m_points.push_back(mth::float2((float)(rand() % w), (float)(rand() % h)));
}

void MapWalker::PlacedPoints()
{
	m_points.push_back(mth::float2(680.0f, 50.0f));
	m_points.push_back(mth::float2(580.0f, 50.0f));
	m_points.push_back(mth::float2(480.0f, 50.0f));
	m_points.push_back(mth::float2(380.0f, 50.0f));
	m_points.push_back(mth::float2(280.0f, 50.0f));
	m_points.push_back(mth::float2(180.0f, 50.0f));
	m_points.push_back(mth::float2(80.0f, 50.0f));
	m_points.push_back(mth::float2(80.0f, 150.0f));
	m_points.push_back(mth::float2(80.0f, 250.0f));
	m_points.push_back(mth::float2(80.0f, 350.0f));
	m_points.push_back(mth::float2(80.0f, 450.0f));
	m_points.push_back(mth::float2(180.0f, 450.0f));
	m_points.push_back(mth::float2(280.0f, 450.0f));
	m_points.push_back(mth::float2(380.0f, 450.0f));
	m_points.push_back(mth::float2(480.0f, 450.0f));
	m_points.push_back(mth::float2(580.0f, 450.0f));
	m_points.push_back(mth::float2(580.0f, 550.0f));
	m_points.push_back(mth::float2(580.0f, 650.0f));
	m_points.push_back(mth::float2(680.0f, 650.0f));
	m_points.push_back(mth::float2(780.0f, 650.0f));
	m_points.push_back(mth::float2(880.0f, 650.0f));
	m_points.push_back(mth::float2(980.0f, 650.0f));
	m_points.push_back(mth::float2(1080.0f, 650.0f));
	m_points.push_back(mth::float2(1180.0f, 650.0f));
	m_points.push_back(mth::float2(1280.0f, 650.0f));

	m_points.push_back(mth::float2(780.0f, 50.0f));
	m_points.push_back(mth::float2(780.0f, 150.0f));
	m_points.push_back(mth::float2(780.0f, 250.0f));
	m_points.push_back(mth::float2(880.0f, 250.0f));
	m_points.push_back(mth::float2(980.0f, 250.0f));
	m_points.push_back(mth::float2(1080.0f, 250.0f));
	m_points.push_back(mth::float2(1080.0f, 150.0f));
	m_points.push_back(mth::float2(1080.0f, 50.0f));
	m_points.push_back(mth::float2(1180.0f, 50.0f));
	m_points.push_back(mth::float2(1280.0f, 50.0f));
}

void MapWalker::Paint(HDC hdc, int w, int h)
{
	PatBlt(hdc, 0, 0, w, h, WHITENESS);

	DrawPoints(hdc);
	DrawBullets(hdc);
	m_robo.Draw(hdc);
}
void MapWalker::Timer(WPARAM wparam)
{
	if (wparam == 1)
	{
		decltype(m_prevTime) currTime = std::chrono::steady_clock::now();
		Update(std::chrono::duration<float>(currTime - m_prevTime).count());
		m_prevTime = currTime;
		AskRedraw();
	}
}
void MapWalker::KeyDown(WPARAM wparam)
{
	m_robo.KeyDown(wparam);
}
void MapWalker::KeyUp(WPARAM wparam)
{
	m_robo.KeyUp(wparam);
}
void MapWalker::MouseMove(int x, int y, int dx, int dy)
{
	m_robo.MouseMove(x, y);
}
void MapWalker::LBtnDown(int x, int y)
{
	m_bullets.push_back(Bullet(m_robo.getPosition(), mth::float2(0.0f, 500.0f)*mth::float2x2::Rotation(m_robo.getAngle())));
}

void MapWalker::DrawPoints(HDC hdc)
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

void MapWalker::Update(float delta)
{
	m_robo.Update(delta);
	m_robo.UpdateSight(m_points);
	UpdateBullets(delta);
}

void MapWalker::UpdateBullets(float delta)
{
	auto b = m_bullets.begin();
	while (b != m_bullets.end())
	{
		auto curr = b;
		b++;
		curr->p += curr->v*delta;
		if (curr->p.x < 0.0f || curr->p.x > 2000.0f
			|| curr->p.y < 0.0f || curr->p.y > 2000.0f)
			m_bullets.erase(curr);
	}
}

void MapWalker::DrawBullets(HDC hdc)
{
	HBRUSH hb = CreateSolidBrush(RGB(20, 20, 202));
	HPEN hp = CreatePen(PS_SOLID, 1, RGB(20, 20, 20));
	SelectObject(hdc, hp);
	SelectObject(hdc, hb);
	float r = 10.0f;
	for (auto b : m_bullets)
		DrawSphere(hdc, b.p, r);
	DeleteObject(hp);
	DeleteObject(hb);
}

MapWalker::MapWalker(HWND hwnd) :WindowClass(hwnd)
{
	RECT rect;
	m_pointRadius = 10.0f;
	SetTimer(hwnd, 1, 20, NULL);
	m_prevTime = std::chrono::steady_clock::now();
	GetClientRect(hwnd, &rect);
	//RandomPoints(16, rect.right, rect.bottom);
	PlacedPoints();
}
MapWalker::~MapWalker()
{
	KillTimer(m_hwnd, 1);
}

MapWalker::Bullet::Bullet(mth::float2 pos, mth::float2 vel) :p(pos), v(vel) {}