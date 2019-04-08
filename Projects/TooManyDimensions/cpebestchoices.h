#pragma once

#include "cameraposeestimator.h"

class CPEBestChoices : public CameraPoseEstimator
{
private:
	std::vector<mth::float3>  m_nextIterPoints3d;
	std::vector<Camera>  m_nextIterCamPose;

private:
	void BestCameraPositions();
	void BestCameraRotations();
	void BestPointspositions();
	void NextIter();

public:
	CPEBestChoices() = default;
	virtual void CreatePoints() override;
	virtual void Estimate(int iter) override;
};

