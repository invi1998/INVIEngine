#pragma once

#include <DirectXMath.h>


namespace EngineMath
{
	// CubeMap的6个面
	enum ECubeMapFace : int
	{
		PositiveX = 0,	// x轴正方向
		NegativeX = 1,	// x轴负方向
		PositiveY = 2,	// y轴正方向
		NegativeY = 3,	// y轴负方向
		PositiveZ = 4,	// z轴正方向
		NegativeZ = 5,	// z轴负方向
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

	// 知道世界坐标（笛卡尔坐标），求出球坐标
	DirectX::XMFLOAT3 CartesianToSpherical(const DirectX::XMFLOAT3& InCartesian)
	{
		DirectX::XMFLOAT3 OutSpherical{};
		// 半径R
		OutSpherical.x = sqrtf(InCartesian.x * InCartesian.x + InCartesian.y * InCartesian.y + InCartesian.z * InCartesian.z);
		// theta
		OutSpherical.y = atan2f(InCartesian.y, InCartesian.x);
		// phi
		OutSpherical.z = acosf(InCartesian.z / OutSpherical.x);
		return OutSpherical;

		//float anglePre = 180.f / DirectX::XM_PI;

		//// 先求出半径
		//float r = sqrtf(InCartesian.x * InCartesian.x + InCartesian.y * InCartesian.y + InCartesian.z * InCartesian.z);
		//// 求出theta
		//float theta = acos(InCartesian.y / r) * anglePre;
		//// 求出phi
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
		float CriticalValue;	// 临界值
	};

	// 右手坐标下，CubeMap的6个面的球坐标的theta和phi的值的范围
	static const std::vector<FCubeMapFaceThetaPhiRange> CubeMapAxialRangeR =
		{
			{ 45.f, 135.f, -45.f, 45.f, 0.f },	// PositiveX phi 属于 0到45 0到-45 临界值为0
			{ 45.f, 135.f, -135.f, 135.f, 180.f },	// NegativeX phi 属于 135到180 -135到-180  临界值为180
			{ 0.f, 45.f, -360.f, 360.f, 0.f },	// PositiveY
			{ 135.f, 180.f, -360.f, 360.f, 0.f },	// NegativeY
			{ 45.f, 135.f, -45.f, -135.f, 0.f },	// PositiveZ
			{ 45.f, 135.f, 45.f, 135.f, 0.f },	// NegativeZ
		};

	// 判断点是否在CubeMap的视口里
	bool IsPointInCubeMapViewport(float theta, float phi, ECubeMapFace face)
	{
		// 判断theta是否在范围内
		if (theta >= CubeMapAxialRangeR[face].ThetaMin && theta <= CubeMapAxialRangeR[face].ThetaMax)
		{
			// 判断phi是否在范围内
			if (phi >= CubeMapAxialRangeR[face].PhiMin && phi <= CubeMapAxialRangeR[face].PhiMax)
			{
				return true;
			}
			else
			{
				// 判断是否在临界值上
				if (phi == CubeMapAxialRangeR[face].CriticalValue)
				{
					return true;
				}
			}
		}
		return false;
	}


	// 传入笛卡尔坐标，返回处于CubeMap的哪个视口里（确保传入的球坐标是CubeMap坐标系下的）
	inline ECubeMapFace GetSampleCubeMapIndex(const DirectX::XMFLOAT3& position)
	{
		// 先将坐标转换为球坐标
		DirectX::XMFLOAT3 spherical = CartesianToSpherical(position);

		// 然后根据球坐标的值，判断处于CubeMap的哪个面
		float theta = position.y;
		float phi = position.z;

		// 判断处于哪个面
		for (int i = 0; i < 6; i++)
		{
			if (IsPointInCubeMapViewport(theta, phi, static_cast<ECubeMapFace>(i)))
			{
				return static_cast<ECubeMapFace>(i);
			}
		}
	}

}
