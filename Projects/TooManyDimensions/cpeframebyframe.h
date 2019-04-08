#pragma once

#include "cameraposeestimator.h"

class CPEFrameByFrame : public CameraPoseEstimator
{
	std::vector<std::vector<mth::float3>> m_inputDirs;

public:
	CPEFrameByFrame() = default;
	virtual void CreatePoints() override;
	virtual void Estimate(int iter) override;
};