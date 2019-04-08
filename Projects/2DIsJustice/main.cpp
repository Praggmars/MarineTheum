#include "mapwalker.h"
#include "compareapp.h"
#include "movepointapp.h"
#include "drawingapp.h"

#define SafeDelete(p)if(p){delete p;p=nullptr;}
template <typename T>
class SafePtr
{
	T* m_ptr;
public:
	SafePtr() :m_ptr(nullptr) {}
	SafePtr(T* ptr) :m_ptr(ptr) {}
	~SafePtr() { Reset(); }
	void Reset() { SafeDelete(m_ptr); };
	operator T*() { return m_ptr; }
	T* operator->() { return m_ptr; }
	T* operator=(T* ptr) { Reset(); m_ptr = ptr; return m_ptr; }
	operator bool() { return m_ptr != nullptr; }
};

#define ID_MAPWALKER 1
#define ID_COMPARE 2
#define ID_MOVEPOINTS 3
#define ID_DRAWING 10
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static SafePtr<WindowClass> app;
	switch (msg)
	{
	case WM_COMMAND:
		switch (wparam)
		{
		case ID_MAPWALKER:
			app = new MapWalker(hwnd);
			break;
		case ID_COMPARE:
			app = new CompareApp(hwnd);
			break;
		case ID_MOVEPOINTS:
			app = new MovePointApp(hwnd);
			break;
		case ID_DRAWING:
			app = new DrawingApp(hwnd);
			break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		return 0;
	case WM_CREATE:
	{
		HMENU menu = CreateMenu();
		AppendMenu(menu, MF_STRING, ID_MAPWALKER, L"Map walker");
		AppendMenu(menu, MF_STRING, ID_COMPARE, L"Compare");
		AppendMenu(menu, MF_STRING, ID_MOVEPOINTS, L"Move points");
		AppendMenu(menu, MF_STRING, ID_DRAWING, L"Drawing");
		SetMenu(hwnd, menu);
	}
	app = new MapWalker(hwnd);
	return 0;
	case WM_PAINT:
		if (app) app->Paint(hwnd);
		return 0;
	case WM_MOUSEMOVE:
	{
		static POINT prevMousePos;
		POINT mousePos;
		mousePos.x = LOWORD(lparam);
		mousePos.y = HIWORD(lparam);
		if (app) app->MouseMove(mousePos.x, mousePos.y, mousePos.x - prevMousePos.x, mousePos.y - prevMousePos.y);
		prevMousePos = mousePos;
		return 0;
	}
	case WM_MOUSEWHEEL:
		if (app) app->MouseWheel(GET_WHEEL_DELTA_WPARAM(wparam) > 0);
		return 0;
	case WM_LBUTTONDOWN:
		if (app) app->LBtnDown(LOWORD(lparam), HIWORD(lparam));
		return 0;
	case WM_LBUTTONUP:
		if (app) app->LBtnUp(LOWORD(lparam), HIWORD(lparam));
		return 0;
	case WM_RBUTTONDOWN:
		if (app) app->RBtnDown(LOWORD(lparam), HIWORD(lparam));
		return 0;
	case WM_RBUTTONUP:
		if (app) app->RBtnUp(LOWORD(lparam), HIWORD(lparam));
		return 0;
	case WM_KEYDOWN:
		if (app) app->KeyDown(wparam);
		return 0;
	case WM_KEYUP:
		if (app) app->KeyUp(wparam);
		return 0;
	case WM_TIMER:
		if (app) app->Timer(wparam);
		return 0;
	case WM_DESTROY:
		//app.Reset();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

int main()
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(wc);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"2DIsJustice";
	RegisterClassEx(&wc);

	HWND hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
		wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, wc.hInstance, NULL);
	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hwnd);

	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}