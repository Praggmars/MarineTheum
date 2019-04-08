#include "pointtracker.h"

PointTracker::PointTracker(int maxObservations) :
	m_observations(),
	m_maxObservations(maxObservations),
	m_latestIndex(0) {}
mth::float3 PointTracker::EstimatePosition()
{
	return mth::LMSLineIntersection(m_observations.data(), (int)m_observations.size());
}
void PointTracker::AddObservation(mth::Line3D observation)
{
	if (m_maxObservations > (int)m_observations.size() || m_maxObservations < 1)
	{
		m_latestIndex = (int)m_observations.size();
		m_observations.push_back(observation);
	}
	else
	{
		m_latestIndex = (m_latestIndex + 1) % m_maxObservations;
		m_observations[m_latestIndex] = observation;
	}
}
void PointTracker::AddObservation(mth::float3 point, mth::float3 dir)
{
	AddObservation(mth::Line3D(point, dir));
}
void PointTracker::Clear()
{
	m_observations.clear();
	m_latestIndex = 0;
}
void PointTracker::SetMaxObservations(int count)
{
	m_maxObservations = count;
	Clear();
}