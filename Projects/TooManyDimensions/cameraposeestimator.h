#pragma once

#include "geometry3d.h"
#include "camera.h"
#include <vector>

class CameraPoseEstimator
{
protected:
	std::vector<std::vector<mth::float2>> m_inputPoints2d;

	std::vector<mth::float3> m_solutionPoints3d;
	std::vector<Camera>  m_solutionCamPose;

	std::vector<mth::float3>  m_estPoints3d;
	std::vector<Camera>  m_estCamPose;

public:
	CameraPoseEstimator() = default;
	virtual void CreatePoints();
	void WriteToFile(const char* filename);
	void WriteEstimationResultToFile(const char* filename);
	virtual void Estimate(int iter) = 0;

	inline std::vector<mth::float3>& getEstimatedPoints() { return m_estPoints3d; }
	inline std::vector<mth::float3>& getSolutionPoints() { return m_solutionPoints3d; }
	inline std::vector<Camera>& getEstimatedCameras() { return m_estCamPose; }
	inline std::vector<Camera>& getSolutionCameras() { return m_solutionCamPose; }
};