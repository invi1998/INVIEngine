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
	DirectX::XMFLOAT3 CartesianToSpherical(const DirectX::XMFLOAT3& InCartesian);

	bool IsAngleRange(float InAngle, float X, float Y);
	fvector_3d GetPointSphericalCoordinates(const fvector_3d& InPoint);
	ECubeMapFace GetSampleCubeMapIndexR(const fvector_3d& InPointPosition);

}
