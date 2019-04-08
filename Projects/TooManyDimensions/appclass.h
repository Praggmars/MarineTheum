#pragma once

#include "camera.h"
#include "pointtracker.h"
#include "cpebestchoices.h"
#include "cpeframebyframe.h"
#include <iostream>
#include <string>
#include <chrono>

class AppClass
{
	struct TrackedPoint
	{
		mth::float3 point;
		PointTracker tracker;

		TrackedPoint(mth::float3 point = mth::float3(), int maxObservations = 5);
	};
	HWND m_hwnd;
	Camera m_cam;
	std::vector<TrackedPoint> m_points;
	std::vector<mth::float3> m_cameraPath;
	decltype(std::chrono::steady_clock::now()) m_prevTime, m_startTime;
	bool m_cameraTracking;
	bool m_visualizing;
	int m_frameCounter;
	float m_cameraSpeed;
	std::unique_ptr<CameraPoseEstimator> m_cpe = std::make_unique<CPEBestChoices>();
	std::vector<mth::float3> m_torus;

private:
	void RandimozePoints(int pointCount);
	void DrawCamera(HDC hdc, Camera& cam);
	void DrawCPESolutionPoints(HDC hdc);
	void DrawCPEEstimationPoints(HDC hdc);
	void DrawCPESolutionCameras(HDC hdc);
	void DrawCPEEstimationCameras(HDC hdc);
	void DrawTrackedPoints(HDC hdc);

	void PaintFreeLook(HDC hdc);
	void PaintResult(HDC hdc);
	void SetUp(HWND hwnd);
	void Paint(HWND hwnd);
	void Update(HWND hwnd);

	void ObservePoints();
	void PrintCameraState();
	void DrawCameraPath(HDC hdc);
	void StartCameraTracking();

	void WriteTrackingToFile();

public:
	int Run();
	LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};