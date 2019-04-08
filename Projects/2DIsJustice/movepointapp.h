#pragma once

#include "windowclass.h"
#include "robot.h"
#include "geometry2d.h"
#include <vector>

class MovePointApp :public WindowClass
{
	float m_pointRadius;
	mth::float2 *m_selectedPoint;
	Robot* m_selectedRobo;

	std::vector<mth::float2> m_inputPoints, m_estPoints;
	Robot m_inputRobo, m_estRobo;

private:
	void DrawRobots(HDC hdc);
	void DrawPoints(HDC hdc);

	void EstimateRoboPose();

public:
	MovePointApp(HWND hwnd);

	virtual void Paint(HDC hdc, int w, int h) override;
	virtual void MouseMove(int x, int y, int dx, int dy) override;
	virtual void LBtnDown(int x, int y) override;
	virtual void LBtnUp(int x, int y) override;
	virtual void MouseWheel(bool up) override;
};