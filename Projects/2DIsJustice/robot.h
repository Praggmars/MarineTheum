#pragma once

#include "linalg.h"
#include "windowclass.h"
#include <vector>

class Robot
{
	std::vector<float> m_sight;

	mth::float2 m_roboPos;
	mth::float2 m_mousePos;
	float m_roboRadius;
	float m_roboVisionHalfSize;
	float m_fov;
	float m_lineLen;
	float m_angle;

	float m_speed;
	bool m_APressed;
	bool m_SPressed;
	bool m_DPressed;
	bool m_WPressed;

private:
	void UpdateAngleLookAtCursor();

public:
	Robot();
	void UpdateSight(std::vector<mth::float2>& points);
	void Update(float delta);
	void Draw(HDC hdc);

	bool GlobalToScreen(mth::float2 p, float& out);
	float GlobalToAngle(mth::float2 p);
	mth::float2 ScreenToGlobal(float s, float dist = 1.0f);
	mth::float2 ScreenToRelative(float s, float dist = 1.0f);

	void MouseMove(int x, int y);
	void KeyDown(WPARAM key);
	void KeyUp(WPARAM key);

	void CopySight(Robot& other);

	float getSightAngle(unsigned i);

	inline float getAngle() { return m_angle; }
	inline void setAngle(float angle) { m_angle = angle; }
	inline mth::float2 getPosition() { return m_roboPos; }
	inline void setPosition(mth::float2 pos) { m_roboPos = pos; }
	inline float getRoboRadius() { return m_roboRadius; }
	mth::float2 getEstimationGlobal(unsigned i, float dist = 1.0f);
	mth::float2 getEstimationRelative(unsigned i, float dist = 1.0f);
};