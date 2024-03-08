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

	DirectX::XMFLOAT4 ToFloat4(const fvector_4d& InV4d);
	fvector_3d ToVector3d(const DirectX::XMFLOAT3& InV3d);

	// 知道世界坐标（笛卡尔坐标），求出球坐标
	DirectX::XMFLOAT3 CartesianToSpherical(const DirectX::XMFLOAT3& InCartesian);

	bool IsAngleRange(float InAngle, float X, float Y);
	fvector_3d GetPointSphericalCoordinates(const fvector_3d& InPoint);
	ECubeMapFace GetSampleCubeMapIndexR(const fvector_3d& InPointPosition);

	int GetSample8CubeIndex(const fvector_3d& InPointPosition);	// 获取8个CubeMap的索引

	void BuildMatrix(DirectX::XMFLOAT4X4& OutMatrix, const DirectX::XMFLOAT3& InPosition, const DirectX::XMFLOAT3& InScale, const DirectX::XMFLOAT3& Right, const DirectX::XMFLOAT3& Up, const DirectX::XMFLOAT3& Forward);

	inline bool IsRange(float InValue, float MinV, float MaxV)
	{
		return InValue >= MinV && InValue <= MaxV;
	}

	// 通过欧拉角构建旋转矩阵的逆矩阵
	void BuildInverseMatrix(DirectX::XMMATRIX& OutMatrix, const DirectX::XMFLOAT3& InPosition, const DirectX::XMFLOAT3& InScale, const DirectX::XMFLOAT3& Right, const DirectX::XMFLOAT3& Up, const DirectX::XMFLOAT3& Forward);

	// 线性差值
	template<typename T>
	static T Lerp(const T& A, const T& B, float Alpha)
	{
		return A + Alpha * (B - A);	// A + Alpha * (B - A) = A * (1 - Alpha) + B * Alpha
	}
}
