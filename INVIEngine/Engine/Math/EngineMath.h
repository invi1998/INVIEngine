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
	DirectX::XMFLOAT3 CartesianToSpherical(const DirectX::XMFLOAT3& InCartesian);

	bool IsAngleRange(float InAngle, float X, float Y);
	fvector_3d GetPointSphericalCoordinates(const fvector_3d& InPoint);
	ECubeMapFace GetSampleCubeMapIndexR(const fvector_3d& InPointPosition);

}
