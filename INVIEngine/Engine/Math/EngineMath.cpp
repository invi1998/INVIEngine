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
}
