#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class WindowClass
{
protected:
	HWND m_hwnd;

protected:
	void AskRedraw();

public:
	WindowClass(HWND hwnd);
	virtual ~WindowClass();
	void Paint(HWND hwnd);
	virtual void Paint(HDC hdc, int w, int h);
	virtual void KeyDown(WPARAM wparam);
	virtual void KeyUp(WPARAM wparam);
	virtual void MouseMove(int x, int y, int dx, int dy);
	virtual void LBtnDown(int x, int y);
	virtual void LBtnUp(int x, int y);
	virtual void RBtnDown(int x, int y);
	virtual void RBtnUp(int x, int y);
	virtual void MouseWheel(bool up);
	virtual void Timer(WPARAM wparam);
};