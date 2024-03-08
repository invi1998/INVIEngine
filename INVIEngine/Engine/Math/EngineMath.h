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

	DirectX::XMFLOAT4 ToFloat4(const fvector_4d& InV4d);
	fvector_3d ToVector3d(const DirectX::XMFLOAT3& InV3d);

	// ֪���������꣨�ѿ������꣩�����������
	DirectX::XMFLOAT3 CartesianToSpherical(const DirectX::XMFLOAT3& InCartesian);

	bool IsAngleRange(float InAngle, float X, float Y);
	fvector_3d GetPointSphericalCoordinates(const fvector_3d& InPoint);
	ECubeMapFace GetSampleCubeMapIndexR(const fvector_3d& InPointPosition);

	int GetSample8CubeIndex(const fvector_3d& InPointPosition);	// ��ȡ8��CubeMap������

	void BuildMatrix(DirectX::XMFLOAT4X4& OutMatrix, const DirectX::XMFLOAT3& InPosition, const DirectX::XMFLOAT3& InScale, const DirectX::XMFLOAT3& Right, const DirectX::XMFLOAT3& Up, const DirectX::XMFLOAT3& Forward);

	inline bool IsRange(float InValue, float MinV, float MaxV)
	{
		return InValue >= MinV && InValue <= MaxV;
	}

	// ͨ��ŷ���ǹ�����ת����������
	void BuildInverseMatrix(DirectX::XMMATRIX& OutMatrix, const DirectX::XMFLOAT3& InPosition, const DirectX::XMFLOAT3& InScale, const DirectX::XMFLOAT3& Right, const DirectX::XMFLOAT3& Up, const DirectX::XMFLOAT3& Forward);

	// ���Բ�ֵ
	template<typename T>
	static T Lerp(const T& A, const T& B, float Alpha)
	{
		return A + Alpha * (B - A);	// A + Alpha * (B - A) = A * (1 - Alpha) + B * Alpha
	}
}
