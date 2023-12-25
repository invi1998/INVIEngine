#pragma once

#include <DirectXMath.h>


namespace EngineMath
{
	// CubeMap��6����
	enum ECubeMapFace : int
	{
		PositiveX = 0,	// x��������
		NegativeX = 1,	// x�Ḻ����
		PositiveY = 2,	// y��������
		NegativeY = 3,	// y�Ḻ����
		PositiveZ = 4,	// z��������
		NegativeZ = 5,	// z�Ḻ����
	};

	static DirectX::XMFLOAT4X4 IdentityMatrix4x4()
	{
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	static DirectX::XMFLOAT3 ToFloat3(const fvector_3d& InV3d)
	{
		return DirectX::XMFLOAT3(InV3d.x, InV3d.y, InV3d.z);
	}

	// ֪���������꣨�ѿ������꣩�����������
	DirectX::XMFLOAT3 CartesianToSpherical(const DirectX::XMFLOAT3& InCartesian)
	{
		DirectX::XMFLOAT3 OutSpherical{};
		// �뾶R
		OutSpherical.x = sqrtf(InCartesian.x * InCartesian.x + InCartesian.y * InCartesian.y + InCartesian.z * InCartesian.z);
		// theta
		OutSpherical.y = atan2f(InCartesian.y, InCartesian.x);
		// phi
		OutSpherical.z = acosf(InCartesian.z / OutSpherical.x);
		return OutSpherical;

		//float anglePre = 180.f / DirectX::XM_PI;

		//// ������뾶
		//float r = sqrtf(InCartesian.x * InCartesian.x + InCartesian.y * InCartesian.y + InCartesian.z * InCartesian.z);
		//// ���theta
		//float theta = acos(InCartesian.y / r) * anglePre;
		//// ���phi
		//float phi = atan2f(InCartesian.z, InCartesian.x) * anglePre;

		//return DirectX::XMFLOAT3{ r, theta, phi };
	}

	struct FCubeMapFaceThetaPhiRange
	{
		FCubeMapFaceThetaPhiRange(float inThetaMin, float inThetaMax, float inPhiMin, float inPhiMax, float inCriticalValue)
			: ThetaMin(inThetaMin)
			, ThetaMax(inThetaMax)
			, PhiMin(inPhiMin)
			, PhiMax(inPhiMax)
			, CriticalValue(inCriticalValue)
		{}

		float ThetaMin;
		float ThetaMax;
		float PhiMin;
		float PhiMax;
		float CriticalValue;	// �ٽ�ֵ
	};

	// ���������£�CubeMap��6������������theta��phi��ֵ�ķ�Χ
	static const std::vector<FCubeMapFaceThetaPhiRange> CubeMapAxialRangeR =
		{
			{ 45.f, 135.f, -45.f, 45.f, 0.f },	// PositiveX phi ���� 0��45 0��-45 �ٽ�ֵΪ0
			{ 45.f, 135.f, -135.f, 135.f, 180.f },	// NegativeX phi ���� 135��180 -135��-180  �ٽ�ֵΪ180
			{ 0.f, 45.f, -360.f, 360.f, 0.f },	// PositiveY
			{ 135.f, 180.f, -360.f, 360.f, 0.f },	// NegativeY
			{ 45.f, 135.f, -45.f, -135.f, 0.f },	// PositiveZ
			{ 45.f, 135.f, 45.f, 135.f, 0.f },	// NegativeZ
		};

	// �жϵ��Ƿ���CubeMap���ӿ���
	bool IsPointInCubeMapViewport(float theta, float phi, ECubeMapFace face)
	{
		// �ж�theta�Ƿ��ڷ�Χ��
		if (theta >= CubeMapAxialRangeR[face].ThetaMin && theta <= CubeMapAxialRangeR[face].ThetaMax)
		{
			// �ж�phi�Ƿ��ڷ�Χ��
			if (phi >= CubeMapAxialRangeR[face].PhiMin && phi <= CubeMapAxialRangeR[face].PhiMax)
			{
				return true;
			}
			else
			{
				// �ж��Ƿ����ٽ�ֵ��
				if (phi == CubeMapAxialRangeR[face].CriticalValue)
				{
					return true;
				}
			}
		}
		return false;
	}


	// ����ѿ������꣬���ش���CubeMap���ĸ��ӿ��ȷ���������������CubeMap����ϵ�µģ�
	inline ECubeMapFace GetSampleCubeMapIndex(const DirectX::XMFLOAT3& position)
	{
		// �Ƚ�����ת��Ϊ������
		DirectX::XMFLOAT3 spherical = CartesianToSpherical(position);

		// Ȼ������������ֵ���жϴ���CubeMap���ĸ���
		float theta = position.y;
		float phi = position.z;

		// �жϴ����ĸ���
		for (int i = 0; i < 6; i++)
		{
			if (IsPointInCubeMapViewport(theta, phi, static_cast<ECubeMapFace>(i)))
			{
				return static_cast<ECubeMapFace>(i);
			}
		}
	}

}
