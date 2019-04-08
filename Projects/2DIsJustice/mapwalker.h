#pragma once
#include "robot.h"
#include <chrono>
#include <list>

class MapWalker :public WindowClass
{
	struct Bullet
	{
		mth::float2 p;
		mth::float2 v;

		Bullet(mth::float2 pos, mth::float2 vel);
	};

	decltype(std::chrono::steady_clock::now()) m_prevTime;

	std::vector<mth::float2> m_points;
	float m_pointRadius;

	std::list<Bullet> m_bullets;

	Robot m_robo;

private:
	void RandomPoints(int count, int w, int h);
	void PlacedPoints();

	void DrawPoints(HDC hdc);

	void Update(float delta);

	void UpdateBullets(float delta);
	void DrawBullets(HDC hdc);
	
public:
	MapWalker(HWND hwnd);
	~MapWalker();

	virtual void Paint(HDC hdc, int w, int h) override;
	virtual void Timer(WPARAM wparam) override;
	virtual void KeyDown(WPARAM wparam) override;
	virtual void KeyUp(WPARAM wparam) override;
	virtual void MouseMove(int x, int y, int dx, int dy) override;
	virtual void LBtnDown(int x, int y) override;
};