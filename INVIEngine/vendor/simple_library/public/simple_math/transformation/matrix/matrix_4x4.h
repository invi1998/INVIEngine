#pragma once

struct fmatrix_4x4
{
	float m11; float m12; float m13; float m14;
	float m21; float m22; float m23; float m24;
	float m31; float m32; float m33; float m34;
	float m41; float m42; float m43; float m44;

public:
	fmatrix_4x4();

	fmatrix_4x4(
		float in_m11, float in_m12, float in_m13, float in_m14,
		float in_m21, float in_m22, float in_m23, float in_m24,
		float in_m31, float in_m32, float in_m33, float in_m34,
		float in_m41, float in_m42, float in_m43, float in_m44);

	void operator+=(const fmatrix_4x4& a)
	{
		*this = *this + a;
	}

	fmatrix_4x4 operator+(const fmatrix_4x4& a)const
	{
		return fmatrix_4x4(
			m11 + a.m11, m12 + a.m12, m13 + a.m13, m14 + a.m14,
			m21 + a.m21, m22 + a.m22, m23 + a.m23, m24 + a.m24,
			m31 + a.m31, m32 + a.m32, m33 + a.m33, m34 + a.m34,
			m41 + a.m41, m42 + a.m42, m43 + a.m43, m44 + a.m44);
	}

	void operator-=(const fmatrix_4x4& a)
	{
		*this = *this - a;
	}

	fmatrix_4x4 operator-(const fmatrix_4x4& a)const
	{
		return fmatrix_4x4(
			m11 - a.m11, m12 - a.m12, m13 - a.m13, m14 - a.m14,
			m21 - a.m21, m22 - a.m22, m23 - a.m23, m24 - a.m24,
			m31 - a.m31, m32 - a.m32, m33 - a.m33, m34 - a.m34,
			m41 - a.m41, m42 - a.m42, m43 - a.m43, m44 - a.m44);
	}

	fmatrix_4x4 operator*=(const fmatrix_4x4& a)
	{
		*this = *this * a;
	}

	fmatrix_4x4 operator*(const fmatrix_4x4& a)const
	{
		return fmatrix_4x4(
			m11 * a.m11 + m12 * a.m12 + m13 * a.m13 + m14 * a.m14,
			m11 * a.m21 + m12 * a.m22 + m13 * a.m23 + m14 * a.m24,
			m11 * a.m31 + m12 * a.m32 + m13 * a.m33 + m14 * a.m34,
			m11 * a.m41 + m12 * a.m42 + m13 * a.m43 + m14 * a.m44,
			
			m21* a.m11 + m22 * a.m12 + m23 * a.m13 + m24 * a.m14,
			m21* a.m21 + m22 * a.m22 + m23 * a.m23 + m24 * a.m24,
			m21* a.m31 + m22 * a.m32 + m23 * a.m33 + m24 * a.m34,
			m21* a.m41 + m22 * a.m42 + m23 * a.m43 + m24 * a.m44,
			
			m31* a.m11 + m32 * a.m12 + m33 * a.m13 + m34 * a.m14,
			m31* a.m21 + m32 * a.m22 + m33 * a.m23 + m34 * a.m24,
			m31* a.m31 + m32 * a.m32 + m33 * a.m33 + m34 * a.m34,
			m31* a.m41 + m32 * a.m42 + m33 * a.m43 + m34 * a.m44,
		
			m41* a.m11 + m42 * a.m12 + m43 * a.m13 + m44 * a.m14,
			m41* a.m21 + m42 * a.m22 + m43 * a.m23 + m44 * a.m24,
			m41* a.m31 + m42 * a.m32 + m43 * a.m33 + m44 * a.m34,
			m41* a.m41 + m42 * a.m42 + m43 * a.m43 + m44 * a.m44);
	}

	//行列式
	float Determinant()const;

	//单位化
	static fmatrix_4x4 identity();
};