#pragma once

#include "linalg.h"
#include <Windows.h>

class Camera
{
	mth::float3x3 m_rot;
	POINT m_prevMousePosition;
	char m_keyFlags;

public:
	mth::float3 pos;
	mth::float3 rot;

public:
	Camera();
	void UpdateMatrices();
	void Move(float delta);
	void MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam);
	mth::float2 GlobalToScreen(mth::float3 p);
	mth::float3 GlobalToGlobalDir(mth::float3 p);
	mth::float3 GlobalToRelativeDir(mth::float3 p);
	mth::float3 ScreenToGlobal(mth::float2 p, float cameraZDist);
	mth::float3 ScreenToGlobalDir(mth::float2 p);
	mth::float3 ScreenToRelativeDir(mth::float2 p);
};