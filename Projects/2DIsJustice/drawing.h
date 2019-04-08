#pragma once

#include "geometry2d.h"
#include <Windows.h>

void DrawSphere(HDC hdc, mth::float2 p, float r);

void DrawEndlessLine(HDC hdc, mth::float2 p, float a);

void DrawSameAngleSpheres(HDC hdc, mth::float2 p1, mth::float2 p2, float a);