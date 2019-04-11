#pragma once

#include "windowclass.h"
#include "drawing.h"

class DrawingApp :public WindowClass
{
	mth::Circle2D m_circle1, m_circle2;
	mth::Line2D m_line1, m_line2;

public:
	DrawingApp(HWND hwnd);
	virtual ~DrawingApp() override;

	virtual void Paint(HDC hdc, int w, int h) override;
	virtual void MouseMove(int x, int y, int dx, int dy) override;
	virtual void LBtnDown(int x, int y) override;
	virtual void LBtnUp(int x, int y) override;
};