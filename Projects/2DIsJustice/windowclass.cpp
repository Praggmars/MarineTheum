#include "windowclass.h"

void WindowClass::AskRedraw()
{
	InvalidateRect(m_hwnd, NULL, FALSE);
}

WindowClass::WindowClass(HWND hwnd) :m_hwnd(hwnd) {}

WindowClass::~WindowClass() {}

void WindowClass::Paint(HWND hwnd)
{
	PAINTSTRUCT ps;
	RECT rect;
	GetClientRect(hwnd, &rect);
	HDC hdc = BeginPaint(hwnd, &ps);
	HDC hmem = CreateCompatibleDC(hdc);
	HBITMAP hbmp = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	SelectObject(hmem, hbmp);
	Paint(hmem, rect.right, rect.bottom);
	BitBlt(hdc, 0, 0, rect.right, rect.bottom, hmem, 0, 0, SRCCOPY);
	DeleteObject(hbmp);
	DeleteDC(hmem);
	EndPaint(hwnd, &ps);
}

void WindowClass::Paint(HDC hdc, int w, int h) {}
void WindowClass::KeyDown(WPARAM wparam) {}
void WindowClass::KeyUp(WPARAM wparam) {}
void WindowClass::MouseMove(int x, int y, int dx, int dy) {}
void WindowClass::LBtnDown(int x, int y) {}
void WindowClass::LBtnUp(int x, int y) {}
void WindowClass::RBtnDown(int x, int y) {}
void WindowClass::RBtnUp(int x, int y) {}
void WindowClass::MouseWheel(bool up) {}
void WindowClass::Timer(WPARAM wparam) {}