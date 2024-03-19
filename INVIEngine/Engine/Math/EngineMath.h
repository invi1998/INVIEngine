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

		FaceMAX = 6	// 未定义方向
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
		return A + (B - A) * Alpha;	// A + Alpha * (B - A) = A * (1 - Alpha) + B * Alpha
	}

	// 构建旋转矩阵
	void BuildRotationMatrix(DirectX::XMMATRIX& OutMatrix, const DirectX::XMFLOAT3& InRight, const DirectX::XMFLOAT3& InUp, const DirectX::XMFLOAT3& InForward);
	void BuildRotationMatrix(fmatrix_3x3& OutMatrix, const DirectX::XMFLOAT3& InRight, const DirectX::XMFLOAT3& InUp, const DirectX::XMFLOAT3& InForward);
	void BuildRotationMatrix(fmatrix_3x3& OutMatrix, const fvector_3d& InRight, const fvector_3d& InUp, const fvector_3d& InForward);
	frotator BuildRotationMatrix(const fvector_3d& InForward, const fvector_3d& InUp = { 0.f, 1.f, 0.f });

	// 构建四元数
	DirectX::XMVECTOR BuildQuaternion(const DirectX::XMFLOAT3& InForward, const DirectX::XMFLOAT3& InUp = { 0.f, 1.f, 0.f });
	fquat BuildQuaternionFQuat(const fvector_3d& InForward, const fvector_3d& InUp = { 0.f, 1.f, 0.f });
	frotator ToDXRotator(const frotator& InRotator);

	template<typename T>
	T GetRandomNumberGenerator(T InMin, T InMax)
	{
		// 创建一个随机数引擎对象并设置种子（可以使用时间作为种子）
		std::mt19937 mt(std::random_device{}());

		// 创建一个随机数分布对象
		std::uniform_real_distribution<T> dist(InMin, InMax);

		// 生成随机数
		T RandomNumber = dist(mt);

		return RandomNumber;
	}
}
