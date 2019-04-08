#include "cpebestchoices.h"

void CPEBestChoices::BestCameraPositions()
{
	std::vector<mth::Line3D> lines(m_estPoints3d.size());
	for (int f = 0; f < m_estCamPose.size(); f++)
	{
		for (int p = 0; p < m_estPoints3d.size(); p++)
		{
			lines[p].point = m_estPoints3d[p];
			lines[p].dir = m_estCamPose[f].ScreenToGlobalDir(m_inputPoints2d[f][p]);
			if (lines[p].dir.isZeroVector())
				lines[p].dir.z = 1.0f;
		}
		m_nextIterCamPose[f].pos = mth::LMSLineIntersection(lines.data(), (int)lines.size());
	}
}
void CPEBestChoices::BestCameraRotations()
{
	for (int f = 0; f < m_estCamPose.size(); f++)
	{
		mth::float3 rot;
		for (int p = 0; p < m_estPoints3d.size(); p++)
			rot += mth::float3x3::RotateUnitVector(
				m_estCamPose[f].GlobalToGlobalDir(m_estPoints3d[p]).Normalized(),
				m_estCamPose[f].ScreenToGlobalDir(m_inputPoints2d[f][p]).Normalized()
			).ToRotationAngles();
		m_nextIterCamPose[f].rot = rot / (float)m_estPoints3d.size();
	}
}
void CPEBestChoices::BestPointspositions()
{
	std::vector<mth::Line3D> lines(m_estCamPose.size());
	for (int p = 0; p < m_estPoints3d.size(); p++)
	{
		for (int f = 0; f < m_estCamPose.size(); f++)
		{
			lines[f].point = m_estCamPose[f].pos;
			lines[f].dir = m_estCamPose[f].ScreenToGlobalDir(m_inputPoints2d[f][p]);
			if (lines[f].dir.isZeroVector())
				lines[f].dir.z = 1.0f;
		}
		m_nextIterPoints3d[p] = mth::LMSLineIntersection(lines.data(), (int)lines.size());
	}
}
void CPEBestChoices::NextIter()
{
	float step = 0.1f;
	for (int f = 0; f < m_estCamPose.size(); f++)
	{
		m_estCamPose[f].pos = step * m_nextIterCamPose[f].pos + (1.0f - step) * m_estCamPose[f].pos;
		m_estCamPose[f].rot = step * m_nextIterCamPose[f].rot + (1.0f - step) * m_estCamPose[f].rot;
		m_estCamPose[f].UpdateMatrices();
	}
	for (int p = 0; p < m_estPoints3d.size(); p++)
	{
		m_estPoints3d[p] = step * m_nextIterPoints3d[p] + (1.0f - step) * m_estPoints3d[p];
	}
}

void CPEBestChoices::CreatePoints()
{
	CameraPoseEstimator::CreatePoints();
	m_nextIterCamPose.resize(m_solutionCamPose.size());
	m_nextIterPoints3d.resize(m_solutionPoints3d.size());
}

void CPEBestChoices::Estimate(int iter)
{
	for (int i = 0; i < iter; i++)
	{
		BestCameraPositions();
		BestCameraRotations();
		BestPointspositions();
		NextIter();
	}
}
