#include "cameraposeestimator.h"
#include <fstream>

void CameraPoseEstimator::CreatePoints()
{
	m_solutionPoints3d.resize(16);
	for (auto& p : m_solutionPoints3d)
		p = mth::float3(mth::Random(-1.0f, 1.0f), mth::Random(-1.0f, 1.0f), mth::Random(-1.0f, 1.0f));
	m_estPoints3d.resize(m_solutionPoints3d.size());

	for (float t = 0.0f; t < 7.0f; t += 0.25f)
	{
		Camera cam;
		float time = t + 3.0f;
		cam.pos.x = 1.0f - 4.0f*sinf(time*0.2f);
		cam.pos.y = 0.1f*cosf(time*2.5f);
		cam.pos.z = -3.0f + 5.0f*sinf(time*0.1f);
		cam.rot.x = 0.05f*cosf(time*3.0f);
		cam.rot.y = time * 0.25f;
		cam.rot.z = 0.0f;
		cam.UpdateMatrices();
		m_solutionCamPose.push_back(cam);
	}
	m_estCamPose.resize(m_solutionCamPose.size());

	m_inputPoints2d.resize(m_solutionCamPose.size());
	for (int f = 0; f < m_solutionCamPose.size(); f++)
	{
		m_inputPoints2d[f].resize(m_solutionPoints3d.size());
		for (int p = 0; p < m_solutionPoints3d.size(); p++)
		{
			m_inputPoints2d[f][p] = m_solutionCamPose[f].GlobalToScreen(m_solutionPoints3d[p]);
		}
	}

	/*float epsilon = 0.05f;
	for (int f = 0; f < m_solutionCamPose.size(); f++)
	{
		m_estCamPose[f].pos = m_solutionCamPose[f].pos + mth::float3(mth::Random(-epsilon, epsilon), mth::Random(-epsilon, epsilon), mth::Random(-epsilon, epsilon));
		m_estCamPose[f].rot = m_solutionCamPose[f].rot + mth::float3(mth::Random(-epsilon, epsilon), mth::Random(-epsilon, epsilon), mth::Random(-epsilon, epsilon));
		m_estCamPose[f].UpdateMatrices();
	}*/

	for (int p = 0; p < m_estPoints3d.size(); p++)
		m_estPoints3d[p] = m_estCamPose[0].ScreenToGlobal(m_inputPoints2d[0][p], 1.0f);
}

void CameraPoseEstimator::WriteToFile(const char* filename)
{
	std::ofstream of(filename, std::ios::out);
	of << (int)m_solutionPoints3d.size() << std::endl << std::endl;
	for (int f = 0; f < m_solutionCamPose.size(); f++)
	{
		for (int p = 0; p < m_solutionPoints3d.size(); p++)
		{
			of << m_inputPoints2d[f][p] << std::endl;
		}
		of << std::endl;
	}
}

void CameraPoseEstimator::WriteEstimationResultToFile(const char* filename)
{
	std::ofstream outfile(filename, std::ios::out);
	int pointCount = (int)m_estPoints3d.size();
	outfile << pointCount << std::endl << std::endl;
	for (int f = 0; f < m_estCamPose.size(); f++)
	{
		for (int p = 0; p < m_estPoints3d.size(); p++)
			outfile << m_estCamPose[f].GlobalToScreen(m_estPoints3d[p]) << std::endl;
		outfile << std::endl;
	}
}