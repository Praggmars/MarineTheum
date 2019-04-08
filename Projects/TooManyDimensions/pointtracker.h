#pragma once

#include "geometry3d.h"
#include <vector>

class PointTracker
{
	std::vector<mth::Line3D> m_observations;
	int m_maxObservations;
	int m_latestIndex;

public:
	PointTracker(int maxObservations = 5);
	mth::float3 EstimatePosition();
	void AddObservation(mth::Line3D observation);
	void AddObservation(mth::float3 point, mth::float3 dir);
	void Clear();
	void SetMaxObservations(int count);
};
