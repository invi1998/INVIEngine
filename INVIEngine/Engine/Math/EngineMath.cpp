#include "EngineMinimal.h"

#include "EngineMath.h"

namespace EngineMath
{
	// 右手坐标下，CubeMap的6个面的球坐标的theta和phi的值的范围
	//static const std::vector<FCubeMapFaceThetaPhiRange> CubeMapAxialRangeR =
	//{
	//	{ 45.f, 135.f, -45.f, 45.f, 0.f },	// PositiveX phi 属于 0到45 0到-45 临界值为0
	//	{ 45.f, 135.f, -135.f, 135.f, 180.f },	// NegativeX phi 属于 135到180 -135到-180  临界值为180
	//	{ 0.f, 45.f, -360.f, 360.f, 0.f },	// PositiveY
	//	{ 135.f, 180.f, -360.f, 360.f, 0.f },	// NegativeY
	//	{ 45.f, 135.f, 45.f, 135.f, 0.f },	// PositiveZ
	//	{ 45.f, 135.f, -45.f, -135.f, 0.f },	// NegativeZ
	//};

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

	bool IsAngleRange(float InAngle, float X, float Y)
	{
		if (InAngle >= min(X, Y))
		{
			if (InAngle <= max(X, Y))
			{
				return true;
			}
		}

		return false;
	}

	fvector_3d GetPointSphericalCoordinates(const fvector_3d& InPoint)
	{
		float AnglePre = (180.f / 3.1415926f);

		float R = sqrt(InPoint.x * InPoint.x + InPoint.y * InPoint.y + InPoint.z * InPoint.z);
		float Thata = acos(InPoint.y / R) * AnglePre;
		float Fai = atan2f(InPoint.z, InPoint.x) * AnglePre;

		return fvector_3d(R, Thata, Fai);
	}

	DirectX::XMFLOAT4 ToFloat4(const fvector_4d& InV4d)
	{
		return DirectX::XMFLOAT4(InV4d.x, InV4d.y, InV4d.z, InV4d.w);
	}

	fvector_3d ToVector3d(const DirectX::XMFLOAT3& InV3d)
	{
		return fvector_3d(InV3d.x, InV3d.y, InV3d.z);
	}

	struct FCubeMapAxialRangeR
	{
		FCubeMapAxialRangeR()
			:PositiveX(45.f, 135.f, 45.f, -45.f)//fai 属于 0-45  0--45
			, NegativeX(45.f, 135.f, 135.f, -135.f)//fail 属于 135-180 -135--180
			, PositiveY(0.f, 45.f, 360.f, -360.f)//theta 属于 0-45
			, NegativeY(135.f, 180.f, 360.f, -360.f)//theta 属于 135-180
			, PositiveZ(45.f, 135.f, 45.f, 135.f)
			, NegativeZ(45.f, 135.f, -45.f, -135.f)
		{}

		const fvector_4d PositiveX;
		const fvector_4d NegativeX;

		const fvector_4d PositiveY;
		const fvector_4d NegativeY;

		const fvector_4d PositiveZ;
		const fvector_4d NegativeZ;
	};

	bool IsAngleAxisRange(
		float InAngle,
		float X,
		float Y,
		float InCriticalValue,
		bool bComMin)
	{
		if (bComMin) //比小
		{
			if (InAngle > InCriticalValue)
			{
				if (InAngle <= max(X, Y))
				{
					return true;
				}
			}
			else if (InAngle < InCriticalValue)
			{
				if (InAngle >= min(X, Y))
				{
					return true;
				}
			}
			else if (InAngle == 0.f)
			{
				return true;
			}
		}
		else
		{
			if (InAngle > 0.f) //+
			{
				if (InAngle >= max(X, Y))
				{
					if (InAngle <= InCriticalValue)
					{
						return true;
					}
				}
			}
			else
			{
				if (InAngle <= min(X, Y))
				{
					if (InAngle >= -InCriticalValue)
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool IsPointInCubeMapVieport(
		float InPointTheta,
		float InPointFai,
		fvector_4d InCompareValue,
		int bAxisRange = 0,
		float InCriticalValue = 0.f,
		bool bComMin = false)
	{
		if (IsAngleRange(InPointTheta, InCompareValue.x, InCompareValue.y))
		{
			if (bAxisRange == 0)
			{
				if (IsAngleAxisRange(InPointFai, InCompareValue.z, InCompareValue.w, InCriticalValue, bComMin))
				{
					return true;
				}
			}
			else if (bAxisRange == 1)
			{
				if (InPointFai >= InCompareValue.z)
				{
					if (InPointFai <= InCompareValue.w)
					{
						return true;
					}
				}
			}
			else if (bAxisRange == 2)
			{
				if (InPointFai <= InCompareValue.z)
				{
					if (InPointFai >= InCompareValue.w)
					{
						return true;
					}
				}
			}
			else
			{
				if (IsAngleRange(InPointFai, InCompareValue.z, InCompareValue.w))
				{
					return true;
				}
			}
		}

		return false;
	}

	ECubeMapFace GetSampleCubeMapIndexR(const fvector_3d& InPointPosition)
	{
		static FCubeMapAxialRangeR CubeMapAxialRangeRight;

		//确保它已经转为CubeMapViewport下的坐标
		//转为球面坐标
		fvector_3d Point = GetPointSphericalCoordinates(InPointPosition);

		//球面坐标值
		float PointTheta = Point.y;
		float PointFai = Point.z;

		if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveX, 3, 0.f, true))
		{
			return ECubeMapFace::PositiveX;
		}
		else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeX, 0, 180.f, false))
		{
			return ECubeMapFace::NegativeX;
		}
		else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveY, 3))
		{
			return ECubeMapFace::PositiveY;
		}
		else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeY, 3))
		{
			return ECubeMapFace::NegativeY;
		}
		else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.PositiveZ, 1))
		{
			return ECubeMapFace::PositiveZ;
		}
		else if (IsPointInCubeMapVieport(PointTheta, PointFai, CubeMapAxialRangeRight.NegativeZ, 2))
		{
			return ECubeMapFace::NegativeZ;
		}
	}

	int GetSample8CubeIndex(const fvector_3d& InPointPosition)
	{
		fvector_3d Point = GetPointSphericalCoordinates(InPointPosition);

		// 球面坐标值
		float PointTheta = Point.y;
		float PointFai = Point.z;

		// 使用theta和fai的值来判断物体在哪个面
		// theta的值在0-45和135-180之间，fai的值在45-135之间，物体在PositiveX
		if (IsRange(PointTheta, 0.f, 90.f))
		{
			if (IsRange(PointFai, 0.f, 90.f))
			{
				return 0;	// PositiveX
			}
			else if (IsRange(PointFai, 90.f, 180.f))
			{
				return 1;	// PositiveY
			}
			else if (IsRange(PointFai, -90.f, 0.f))
			{
				return 3;	// PositiveZ
			}
			else if (IsRange(PointFai, -180.f, -90.f))
			{
				return 2;	// NegativeY
			}
		}
		else if (IsRange(PointTheta, 90.f, 180.f))
		{
			if (IsRange(PointFai, 0.f, 90))
			{
				return 4;	// NegativeX
			}
			else if (IsRange(PointFai, 90.f, 180.f))
			{
				return 5;	// NegativeZ
			}
			else if (IsRange(PointFai, -90.f, 0.f))
			{
				return 7;	// NegativeY
			}
			else if (IsRange(PointFai, -180.f, -90.f))
			{
				return 6;	// PositiveZ
			}
		}
		

		return -1;
	}

	void BuildMatrix(DirectX::XMFLOAT4X4& OutMatrix, const DirectX::XMFLOAT3& InPosition,
		const DirectX::XMFLOAT3& InScale, const DirectX::XMFLOAT3& Right, const DirectX::XMFLOAT3& Up,
		const DirectX::XMFLOAT3& Forward)
	{
		OutMatrix = {
					Right.x * InScale.x,	Up.x * InScale.y,	Forward.x * InScale.z,	0.f,
					Right.y * InScale.x,	Up.y * InScale.y,	Forward.y * InScale.z,	0.f,
					Right.z * InScale.x,	Up.z * InScale.y,	Forward.z * InScale.z,	0.f,
					InPosition.x,			InPosition.y,		InPosition.z,			1.f,
				};
	}

	void BuildInverseMatrix(DirectX::XMMATRIX& OutMatrix, const DirectX::XMFLOAT3& InPosition,
		const DirectX::XMFLOAT3& InScale, const DirectX::XMFLOAT3& Right, const DirectX::XMFLOAT3& Up,
		const DirectX::XMFLOAT3& Forward)
	{
		XMFLOAT4X4 Matrix{};
		BuildMatrix(Matrix, InPosition, InScale, Right, Up, Forward);		// 构建矩阵
		XMMATRIX MatrixRIX = XMLoadFloat4x4(&Matrix);	// 转换为XMMatrix
		XMVECTOR Det = XMMatrixDeterminant(MatrixRIX);	// 计算行列式
		OutMatrix = XMMatrixInverse(&Det, MatrixRIX);	// 计算逆矩阵
	}

	void BuildRotationMatrix(DirectX::XMMATRIX& OutMatrix, const DirectX::XMFLOAT3& InRight,
		const DirectX::XMFLOAT3& InUp, const DirectX::XMFLOAT3& InForward)
	{
		OutMatrix = {
			InRight.x, InUp.x, InForward.x, 0.f,
			InRight.y, InUp.y, InForward.y, 0.f,
			InRight.z, InUp.z, InForward.z, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	void BuildRotationMatrix(fmatrix_3x3& OutMatrix, const DirectX::XMFLOAT3& InRight, const DirectX::XMFLOAT3& InUp,
		const DirectX::XMFLOAT3& InForward)
	{
		OutMatrix = {
			InRight.x, InUp.x, InForward.x,
			InRight.y, InUp.y, InForward.y,
			InRight.z, InUp.z, InForward.z,
		};
		
	}

	void BuildRotationMatrix(fmatrix_3x3& OutMatrix, const fvector_3d& InRight, const fvector_3d& InUp,
		const fvector_3d& InForward)
	{
		OutMatrix = {
					InRight.x, InUp.x, InForward.x,
					InRight.y, InUp.y, InForward.y,
					InRight.z, InUp.z, InForward.z,
				};
	}

	frotator BuildRotationMatrix(const fvector_3d& InForward, const fvector_3d& InUp)
	{
		frotator Rotator;

		fvector_3d RightVector = fvector_3d::cross_product(InUp, InForward);
		RightVector.normalize();

		fvector_3d UPVector = fvector_3d::cross_product(InForward, RightVector);
		UPVector.normalize();

		fmatrix_3x3 RotatorMatrix;
		BuildRotationMatrix(RotatorMatrix,
						RightVector,
						UPVector,
						InForward);

		Rotator.inertia_to_object(RotatorMatrix);

		return ToDXRotator(Rotator);
	}

	DirectX::XMVECTOR BuildQuaternion(const DirectX::XMFLOAT3& InForward, const DirectX::XMFLOAT3& InUp)
	{
		XMVECTOR Right = XMVector3Cross(XMLoadFloat3(&InUp), XMLoadFloat3(&InForward));
		Right = XMVector3Normalize(Right);

		XMVECTOR Up = XMVector3Cross(XMLoadFloat3(&InForward), Right);
		Up = XMVector3Normalize(Up);

		XMFLOAT3 RightVector{};
		XMStoreFloat3(&RightVector, Right);
		XMFLOAT3 UpVector{};
		XMStoreFloat3(&UpVector, Up);
		XMFLOAT3 ForwardVector{};
		XMStoreFloat3(&ForwardVector, XMVector3Normalize(XMLoadFloat3(&InForward)));

		XMMATRIX RotationMatrix{};
		BuildRotationMatrix(RotationMatrix, RightVector, UpVector, ForwardVector);

		return XMQuaternionRotationMatrix(RotationMatrix);	// 根据旋转矩阵计算四元数
	}

	fquat BuildQuaternionFQuat(const fvector_3d& InForwardVector, const fvector_3d& InUPVector)
	{
		fquat Quat;

		fvector_3d RightVector = fvector_3d::cross_product(InUPVector, InForwardVector);
		RightVector.normalize();

		fvector_3d UPVector = fvector_3d::cross_product(InForwardVector, RightVector);
		UPVector.normalize();

		fmatrix_3x3 RotatorMatrix;
		BuildRotationMatrix(RotatorMatrix,
			RightVector,
			UPVector,
			InForwardVector);

		math_utils::matrix_to_quat(RotatorMatrix, Quat);

		return Quat;
	}

	frotator ToDXRotator(const frotator& InRotator)
	{
		return frotator(InRotator.roll, InRotator.pitch, InRotator.yaw);
	}
}
