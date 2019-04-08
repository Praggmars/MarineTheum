#include "cpeframebyframe.h"

void CPEFrameByFrame::CreatePoints()
{
	CameraPoseEstimator::CreatePoints();
	m_inputDirs.resize(m_inputPoints2d.size());
	for (int f = 0; f < m_inputPoints2d.size(); f++)
		m_inputDirs[f].resize(m_inputPoints2d[f].size());
}

void CPEFrameByFrame::Estimate(int iter)
{
	mth::float3 middlePointPrev, middlePointNew;
	for (mth::float3& p : m_inputDirs[0])
		middlePointPrev += p;
	for (mth::float3& p : m_inputDirs[1])
		middlePointNew += p;
	mth::float3 translate2d = middlePointNew - middlePointPrev;
	std::vector<mth::float3> translatedNewFrame(m_inputPoints2d[1].size());
	for (mth::float3& p : translatedNewFrame)
		p -= translate2d;
}
