#include "camera.h"
#include <iostream>

void Camera::UpdateMatrices()
{
	mth::float3 up(0.0f, 1.0f, 0.0f), lookAt(0.0f, 0.0f, 1.0f);

	m_rot = mth::float3x3::Rotation(rot);

	lookAt = m_rot * lookAt;
	up = m_rot * up;

	lookAt += pos;

	m_rot = (mth::float3x3)mth::float4x4::LookAt(pos, lookAt, up);
}

void Camera::Move(float delta)
{
	float speed = ((m_keyFlags&(1 << 6)) ? 5.0f : 1.0f)*delta;
	mth::float3 movement;
	if (m_keyFlags&(1 << 0))	//W
		movement.z += speed;
	if (m_keyFlags&(1 << 1))	//A
		movement.x -= speed;
	if (m_keyFlags&(1 << 2))	//S
		movement.z -= speed;
	if (m_keyFlags&(1 << 3))	//D
		movement.x += speed;
	if (m_keyFlags&(1 << 4))	//Ctrl
		movement.y -= speed;
	if (m_keyFlags&(1 << 5))	//Space
		movement.y += speed;

	float s = sinf(rot.y);
	float c = cosf(rot.y);
	pos.x += s * movement.z;
	pos.z += c * movement.z;
	pos.x += c * movement.x;
	pos.z -= s * movement.x;
	pos.y += movement.y;
}

Camera::Camera()
{
	UpdateMatrices();
}
void Camera::MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam)
{
	POINT mousePosition;
	switch (msg)
	{
	case WM_MOUSEMOVE:
		mousePosition.x = LOWORD(lparam);
		mousePosition.y = HIWORD(lparam);
		if (wparam & MK_LBUTTON)
		{
			rot.y += (mousePosition.x - m_prevMousePosition.x)*0.008f;
			rot.x += (mousePosition.y - m_prevMousePosition.y)*0.008f;
		}
		m_prevMousePosition = mousePosition;
		break;
	case WM_KEYDOWN:
		switch (wparam)
		{
		case 'W':
			m_keyFlags |= 1 << 0;
			break;
		case 'A':
			m_keyFlags |= 1 << 1;
			break;
		case 'S':
			m_keyFlags |= 1 << 2;
			break;
		case 'D':
			m_keyFlags |= 1 << 3;
			break;
		case VK_CONTROL:
			m_keyFlags |= 1 << 4;
			break;
		case VK_SPACE:
			m_keyFlags |= 1 << 5;
			break;
		case VK_SHIFT:
			m_keyFlags |= 1 << 6;
			break;
		}
		break;
	case WM_KEYUP:
		switch (wparam)
		{
		case 'W':
			m_keyFlags &= ~(1 << 0);
			break;
		case 'A':
			m_keyFlags &= ~(1 << 1);
			break;
		case 'S':
			m_keyFlags &= ~(1 << 2);
			break;
		case 'D':
			m_keyFlags &= ~(1 << 3);
			break;
		case VK_CONTROL:
			m_keyFlags &= ~(1 << 4);
			break;
		case VK_SPACE:
			m_keyFlags &= ~(1 << 5);
			break;
		case VK_SHIFT:
			m_keyFlags &= ~(1 << 6);
			break;
		}
		break;
	}
}

mth::float2 Camera::GlobalToScreen(mth::float3 p)
{
	p = GlobalToRelativeDir(p);
	if (p.z <= 0.0f)
		return mth::float2(-1000.0f, -1000.0f);
	return mth::float2(
		(p.x / p.z) * 240.0f + 320.0f,
		240.0f - (p.y / p.z) * 240.0f);
}
mth::float3 Camera::ScreenToGlobal(mth::float2 p, float cameraZDist)
{
	return ScreenToGlobalDir(p)*cameraZDist + pos;
}
mth::float3 Camera::GlobalToGlobalDir(mth::float3 p)
{
	return p - pos;
}
mth::float3 Camera::GlobalToRelativeDir(mth::float3 p)
{
	return m_rot * GlobalToGlobalDir(p);
}
mth::float3 Camera::ScreenToGlobalDir(mth::float2 p)
{
	return ScreenToRelativeDir(p) * m_rot;
}
mth::float3 Camera::ScreenToRelativeDir(mth::float2 p)
{
	return mth::float3((p.x - 320.0f) / 240.0f, (240.0f - p.y) / 240.0f, 1.0f);
}