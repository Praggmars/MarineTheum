#include "appclass.h"
#include <thread>
#include <fstream>
#include <iostream>

#define TIMERID 213

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return ((AppClass*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->MessageHandler(hwnd, msg, wparam, lparam);
}
LRESULT CALLBACK InitialWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_CREATE)
	{
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(((CREATESTRUCT*)lparam)->lpCreateParams));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
		return WndProc(hwnd, msg, wparam, lparam);
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void AppClass::RandimozePoints(int pointCount)
{
	m_points.clear();
	for (int i = 0; i < pointCount; i++)
		m_points.push_back(TrackedPoint(mth::float3(mth::Random(-1.0f, 1.0f), mth::Random(-1.0f, 1.0f), mth::Random(-1.0f, 1.0f))));
}

void AppClass::DrawCPESolutionPoints(HDC hdc)
{
	HPEN hp = CreatePen(PS_SOLID, 3, RGB(0x00, 0xff, 0x00));
	SelectObject(hdc, hp);
	for (auto& p : m_cpe->getSolutionPoints())
	{
		mth::float2 s;
		float r = 3.0f;
		s = m_cam.GlobalToScreen(p);
		Ellipse(hdc, (int)(s.x - r), (int)(s.y - r), (int)(s.x + r), (int)(s.y + r));
	}
	DeleteObject(hp);
}
void AppClass::DrawCPEEstimationPoints(HDC hdc)
{
	HPEN hp = CreatePen(PS_SOLID, 2, RGB(0xff, 0x00, 0x00));
	SelectObject(hdc, hp);
	for (auto& p : m_cpe->getEstimatedPoints())
	{
		mth::float2 s;
		float r;
		s = m_cam.GlobalToScreen(p);
		r = 2.0f;
		Ellipse(hdc, (int)(s.x - r), (int)(s.y - r), (int)(s.x + r), (int)(s.y + r));
	}
	DeleteObject(hp);
}
void AppClass::DrawCamera(HDC hdc, Camera& cam)
{
	POINT pts[5];
	float dist = 0.1f * m_cameraSpeed;
	mth::float2 s = m_cam.GlobalToScreen(cam.pos);
	pts[0].x = (int)s.x; pts[0].y = (int)s.y;
	s = m_cam.GlobalToScreen(cam.ScreenToGlobal(mth::float2(0.0f, 0.0f), dist));
	pts[1].x = (int)s.x; pts[1].y = (int)s.y;
	s = m_cam.GlobalToScreen(cam.ScreenToGlobal(mth::float2(0.0f, 480.0f), dist));
	pts[2].x = (int)s.x; pts[2].y = (int)s.y;
	s = m_cam.GlobalToScreen(cam.ScreenToGlobal(mth::float2(640.0f, 480.0f), dist));
	pts[3].x = (int)s.x; pts[3].y = (int)s.y;
	s = m_cam.GlobalToScreen(cam.ScreenToGlobal(mth::float2(640.0f, 0.0f), dist));
	pts[4].x = (int)s.x; pts[4].y = (int)s.y;

	MoveToEx(hdc, pts[0].x, pts[0].y, NULL);
	LineTo(hdc, pts[1].x, pts[1].y);
	LineTo(hdc, pts[2].x, pts[2].y);
	LineTo(hdc, pts[3].x, pts[3].y);
	LineTo(hdc, pts[4].x, pts[4].y);
	LineTo(hdc, pts[1].x, pts[1].y);
	MoveToEx(hdc, pts[2].x, pts[2].y, NULL);
	LineTo(hdc, pts[0].x, pts[0].y);
	LineTo(hdc, pts[3].x, pts[3].y);
	MoveToEx(hdc, pts[0].x, pts[0].y, NULL);
	LineTo(hdc, pts[4].x, pts[4].y);
}
void AppClass::DrawCPESolutionCameras(HDC hdc)
{
	HPEN hp = CreatePen(PS_SOLID, 1, RGB(0x40, 0x40, 0xff));
	SelectObject(hdc, hp);
	for (auto& c : m_cpe->getSolutionCameras())
		DrawCamera(hdc, c);
	DeleteObject(hp);
}
void AppClass::DrawCPEEstimationCameras(HDC hdc)
{
	HPEN hp = CreatePen(PS_SOLID, 1, RGB(0xc0, 0x40, 0x40));
	SelectObject(hdc, hp);
	for (auto& c : m_cpe->getEstimatedCameras())
		DrawCamera(hdc, c);
	DeleteObject(hp);
}

void AppClass::DrawTrackedPoints(HDC hdc)
{
	HPEN greenPen = CreatePen(PS_SOLID, 3, 0x00ff00);
	HPEN redPen = CreatePen(PS_SOLID, 3, 0x0000ff);
	for (auto& tp : m_points)
	{
		mth::float2 p;
		float r;

		SelectObject(hdc, greenPen);
		p = m_cam.GlobalToScreen(tp.point);
		r = 3.0f;
		Ellipse(hdc, (int)(p.x - r), (int)(p.y - r), (int)(p.x + r), (int)(p.y + r));

		SelectObject(hdc, redPen);
		p = m_cam.GlobalToScreen(tp.tracker.EstimatePosition());
		r = 4.0f;
		Ellipse(hdc, (int)(p.x - r), (int)(p.y - r), (int)(p.x + r), (int)(p.y + r));
	}
	DeleteObject(greenPen);
	DeleteObject(redPen);
}

void AppClass::PaintFreeLook(HDC hdc)
{
	DrawCPESolutionPoints(hdc);
	DrawCPEEstimationPoints(hdc);
	DrawCPESolutionCameras(hdc);
	DrawCPEEstimationCameras(hdc);
	DrawCameraPath(hdc);
}
void AppClass::PaintResult(HDC hdc)
{
	Camera tmpCam = m_cam;
	m_cam = m_cpe->getSolutionCameras()[m_frameCounter];
	DrawCPESolutionPoints(hdc);
	m_cam = m_cpe->getEstimatedCameras()[m_frameCounter];
	DrawCPEEstimationPoints(hdc);
	m_frameCounter++;
	if (m_frameCounter >= m_cpe->getSolutionCameras().size())
		m_frameCounter = 0;
	m_cam = tmpCam;
}

void AppClass::SetUp(HWND hwnd)
{
	/*float R = 3.0f, r = 2.0f;
	for (float x = -mth::pi; x < mth::pi; x += mth::pi / 16.0f)
	{
		for (float y = -mth::pi; y < mth::pi; y += mth::pi / 8.0f)
		{
			m_torus.push_back(mth::float3(
				(R + r * cosf(x))*cosf(y),
				(R + r * cosf(x))*sinf(y),
				r*sin(x)
			));
		}
	}*/

	RandimozePoints(16);
	SetTimer(hwnd, TIMERID, 20, NULL);
	m_startTime = std::chrono::steady_clock::now();
	m_prevTime = m_startTime;
	m_cam.pos = mth::float3(-1.5f, 2.3f, -2.0f);
	m_cam.rot = mth::float3(0.7f, 0.3f, 0.0f);
	m_cameraTracking = false;
	m_cpe->CreatePoints();
	m_cameraSpeed = 1.0f;
	m_visualizing = false;
	m_frameCounter = 0;

	std::thread th([this]() {
		for (;;)
		{
			char cmd;
			std::cin >> cmd;
			int cam;
			switch (cmd)
			{
			case 's':
				std::cin >> cam;
				if (cam >= m_cpe->getSolutionCameras().size())
					std::cout << "Number of cameras: " << m_cpe->getSolutionCameras().size() << std::endl;
				else
				{
					m_cam.pos = m_cpe->getSolutionCameras()[cam].pos;
					m_cam.rot = m_cpe->getSolutionCameras()[cam].rot;
				}
				break;
			case 'e':
				std::cin >> cam;
				if (cam >= m_cpe->getEstimatedCameras().size())
					std::cout << "Number of cameras: " << m_cpe->getEstimatedCameras().size() << std::endl;
				else
				{
					m_cam.pos = m_cpe->getEstimatedCameras()[cam].pos;
					m_cam.rot = m_cpe->getEstimatedCameras()[cam].rot;
				}
				break;
			case 't':
				std::cin >> cam;
				SetTimer(m_hwnd, TIMERID, cam, NULL);
				break;
			case 'i':
				std::cin >> cam;
				m_cpe->Estimate(cam);
				break;
			}
		}
	});
	th.detach();
}

void AppClass::Paint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	HDC hmem = CreateCompatibleDC(hdc);
	HBITMAP hbmp = CreateCompatibleBitmap(hdc, 640, 480);
	SelectObject(hmem, hbmp);

	m_visualizing ? PaintResult(hmem) : PaintFreeLook(hmem);

	/*HPEN hp = CreatePen(PS_SOLID, 2, RGB(0xff, 0xff, 0xff));
	SelectObject(hdc, hp);
	for (auto& p : m_torus)
	{
		mth::float2 s;
		float r = 2.0f;
		s = m_cam.GlobalToScreen(p);
		Ellipse(hdc, (int)(s.x - r), (int)(s.y - r), (int)(s.x + r), (int)(s.y + r));
	}
	DeleteObject(hp);*/

	BitBlt(hdc, 0, 0, 640, 480, hmem, 0, 0, SRCCOPY);
	DeleteObject(hbmp);
	DeleteDC(hmem);
	EndPaint(hwnd, &ps);
}
void AppClass::Update(HWND hwnd)
{
	auto currTime = std::chrono::steady_clock::now();
	float delta = std::chrono::duration<float>(currTime - m_prevTime).count();
	float total = std::chrono::duration<float>(currTime - m_startTime).count();
	m_prevTime = currTime;


	if (m_cameraTracking)
	{
		if (total > 7.0f)
			m_cameraTracking = false;
		float time = total + 3.0f;
		m_cam.pos.x = 1.0f - 4.0f*sinf(time*0.2f);
		m_cam.pos.y = 0.1f*cosf(time*2.5f);
		m_cam.pos.z = -3.0f + 5.0f*sinf(time*0.1f);
		m_cam.rot.x = 0.05f*cosf(time*3.0f);
		m_cam.rot.y = time * 0.25f;
		m_cam.rot.z = 0.0f;
		m_cameraPath.push_back(m_cam.pos);
		//std::cout << m_cam.pos << std::endl << m_cam.rot << std::endl << std::endl;
	}
	else
	{
		m_cam.Move(delta*m_cameraSpeed);
	}

	m_cam.UpdateMatrices();

	InvalidateRect(hwnd, NULL, FALSE);
}
void AppClass::ObservePoints()
{
	for (auto& tp : m_points)
	{
		mth::float2 p;
		p = m_cam.GlobalToScreen(tp.point);
		p.x = floorf(p.x);
		p.y = floorf(p.y);
		tp.tracker.AddObservation(mth::Line3D(m_cam.pos, m_cam.ScreenToGlobalDir(p)));
	}
}
void AppClass::PrintCameraState()
{
	std::cout << m_cam.pos << std::endl << m_cam.rot << std::endl << std::endl;
}
void AppClass::DrawCameraPath(HDC hdc)
{
	if (m_cameraPath.size() > 0)
	{
		HPEN whitePen = CreatePen(PS_SOLID, 3, 0xffffff);
		mth::float2 p = m_cam.GlobalToScreen(m_cameraPath[0]);
		MoveToEx(hdc, (int)p.x, (int)p.y, NULL);
		for (auto& pt : m_cameraPath)
		{
			p = m_cam.GlobalToScreen(pt);
			LineTo(hdc, (int)p.x, (int)p.y);
		}
		DeleteObject(whitePen);
	}
}
void AppClass::StartCameraTracking()
{
	if (!m_cameraTracking)
	{
		m_cameraPath.clear();
		m_cameraTracking = true;
		m_startTime = std::chrono::steady_clock::now();
	}
}
void AppClass::WriteTrackingToFile()
{
	std::ofstream of("track.txt", std::ios::out);
	float time = 3.0f;
	of << m_points.size() << std::endl << std::endl;

	while (time < 7.0f)
	{
		m_cam.pos.x = 1.0f - 4.0f*sinf(time*0.2f);
		m_cam.pos.y = 0.1f*cosf(time*2.5f);
		m_cam.pos.z = -3.0f + 5.0f*sinf(time*0.1f);
		m_cam.rot.x = 0.05f*cosf(time*3.0f);
		m_cam.rot.y = time * 0.25f;
		m_cam.rot.z = 0.0f;
		time += 0.2f;

		for (auto& p : m_points)
		{
			mth::float2 sp = m_cam.GlobalToScreen(p.point);
			sp.x = (float)(int)sp.x;
			sp.y = (float)(int)sp.y;
			of << m_cam.ScreenToGlobalDir(sp) << std::endl;
		}
		of << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}
int AppClass::Run()
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(wc);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = InitialWndProc;
	wc.lpszClassName = L"DimensionZ";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&wc);
	RECT rect{};
	rect.right = 640;
	rect.bottom = 480;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	m_hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		650, 250, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, wc.hInstance, (LPVOID)this);
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);
	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
LRESULT AppClass::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	m_cam.MessageHandler(msg, wparam, lparam);
	switch (msg)
	{
	case WM_CREATE:
		SetUp(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		Paint(hwnd);
		return 0;
	case WM_TIMER:
		Update(hwnd);
		return 0;
	case WM_MOUSEMOVE:
		if (wparam & MK_RBUTTON)
		{
			POINT mousePosition;
			mousePosition.x = LOWORD(lparam);
			mousePosition.y = HIWORD(lparam);
			float minDst = 999.99f;
			mth::float3 dir = m_cam.ScreenToGlobalDir(mth::float2((float)mousePosition.x, (float)mousePosition.y));
			for (auto& tp : m_points)
			{
				auto& p = tp.point;
				float dst = mth::DistanceLineDot(m_cam.pos, dir, p);
				if (minDst > dst)
					minDst = dst;
			}
			SetWindowText(hwnd, std::to_wstring(minDst).c_str());
		}
		return 0;
	case WM_KEYDOWN:
		switch (wparam)
		{
		case 'I':
			m_cpe->Estimate(1);
			break;
		case 'T':
			StartCameraTracking();
			break;
		case 'P':
			PrintCameraState();
			break;
		case 'R':
			RandimozePoints(16);
			break;
		case 'O':
			ObservePoints();
			break;
		case 'F':
			m_cpe->WriteToFile("track.txt");
			m_cpe->WriteEstimationResultToFile("est.txt");
			break;
		case 'V':
			m_visualizing = !m_visualizing;
			break;
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		return 0;
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wparam) > 0)
			m_cameraSpeed *= 1.1f;
		else
			m_cameraSpeed /= 1.1f;
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

AppClass::TrackedPoint::TrackedPoint(mth::float3 point, int maxObservations) :
	point(point),
	tracker(maxObservations) {}