#pragma once

#include <DirectXMath.h>


namespace EngineMath
{
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
}
