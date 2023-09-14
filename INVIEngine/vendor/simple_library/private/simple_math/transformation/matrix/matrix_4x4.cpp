#include "../../../../public/simple_math/transformation/matrix/matrix_4x4.h"
#include "../../../../public/simple_math/transformation/matrix/matrix_3x3.h"

fmatrix_4x4::fmatrix_4x4()
{
    *this = identity();
}

fmatrix_4x4::fmatrix_4x4(
    float in_m11, float in_m12, float in_m13, float in_m14, 
    float in_m21, float in_m22, float in_m23, float in_m24, 
    float in_m31, float in_m32, float in_m33, float in_m34, 
    float in_m41, float in_m42, float in_m43, float in_m44)
    : m11(in_m11), m12(in_m12), m13(in_m13), m14(in_m14)
    , m21(in_m21), m22(in_m22), m23(in_m23), m24(in_m24)
    , m31(in_m31), m32(in_m32), m33(in_m33), m34(in_m34)
    , m41(in_m41), m42(in_m42), m43(in_m43), m44(in_m44)
{

}

float fmatrix_4x4::Determinant()const
{
   //œ»ΩµŒ¨
    //m11
    float m11_algebraic_cofactor = fmatrix_3x3(
        m22, m23, m24,
        m32, m33, m34,
        m42, m43, m44).Determinant();
   
    //m12
    float m12_algebraic_cofactor = fmatrix_3x3(
        m21, m23, m24,
        m31, m33, m34,
        m41, m43, m44).Determinant();
    
    //m13
    float m13_algebraic_cofactor = fmatrix_3x3(
        m21, m22, m24,
        m31, m32, m34,
        m41, m42, m44).Determinant();
    
    //m14
    float m14_algebraic_cofactor = fmatrix_3x3(
        m21, m22, m23,
        m31, m32, m33,
        m41, m42, m43).Determinant();

    return  m11 * m11_algebraic_cofactor -
            m12 * m12_algebraic_cofactor +
            m13 * m13_algebraic_cofactor -
            m14 * m13_algebraic_cofactor;
}

fmatrix_4x4 fmatrix_4x4::identity()
{
    return fmatrix_4x4(
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f);
}
