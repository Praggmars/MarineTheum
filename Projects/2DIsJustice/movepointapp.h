#pragma once

#include "windowclass.h"
#include "robot.h"
#include "geometry2d.h"
#include <vector>
#include <chrono>

class MovePointApp :public WindowClass
{
	float m_pointRadius;
	mth::float2 m_grabOffset;
	mth::float2 *m_selectedPoint;
	Robot* m_selectedRobo;

	std::vector<mth::float2> m_inputPoints, m_estPoints;
	Robot m_inputRobo, m_estRobo;

	decltype(std::chrono::steady_clock::now()) m_prevTime;
	bool m_roboUpdateControl;

private:
	void StartUpdateControl();
	void StopUpdateControl();
	void Update(float delta);

	void DrawRobots(HDC hdc);
	void DrawPoints(HDC hdc);

	void EstimateRoboPose();

public:
	MovePointApp(HWND hwnd);
	virtual ~MovePointApp() override;

	virtual void Paint(HDC hdc, int w, int h) override;
	virtual void MouseMove(int x, int y, int dx, int dy) override;
	virtual void KeyDown(WPARAM wparam) override;
	virtual void KeyUp(WPARAM wparam) override;
	virtual void LBtnDown(int x, int y) override;
	virtual void LBtnUp(int x, int y) override;
	virtual void MouseWheel(bool up) override;
	virtual void Timer(WPARAM wparam) override;
};