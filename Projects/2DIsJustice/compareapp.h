#pragma once

#include "robot.h"

class CompareApp :public WindowClass
{
	Robot m_robo1, m_robo2;
	Robot *m_selectedRobo;
	std::vector<mth::float2> m_points;
	float m_pointRadius;

private:
	void DrawPoints(HDC hdc);
	void DrawEstimatedPoints(HDC hdc);

	bool isRoboClicked(Robot& robo, mth::float2 p);

	void Draw3PointsSameAngles(HDC hdc);
	
public:
	CompareApp(HWND hwnd);

	virtual void Paint(HDC hdc, int w, int h) override;
	virtual void MouseMove(int x, int y, int dx, int dy) override;
	virtual void LBtnDown(int x, int y) override;
	virtual void LBtnUp(int x, int y) override;
	virtual void MouseWheel(bool up) override;
};