#include "..\..\public\simple_math\math_utils.h"

namespace math_utils
{
    fvector_3d mul(const fvector_2d& in_3d, const fmatrix_3x3& in_matrix_3x3)
    {
        return mul(fvector_3d(in_3d.x, in_3d.y, 1.f), in_matrix_3x3);
    }

    fvector_3d mul(const fvector_3d& in_3d, const fmatrix_3x3& in_matrix_3x3)
    {
        return fvector_3d(
            in_3d.x * in_matrix_3x3.m11 + in_3d.y * in_matrix_3x3.m21 + in_3d.z * in_matrix_3x3.m31,
            in_3d.x * in_matrix_3x3.m12 + in_3d.y * in_matrix_3x3.m22 + in_3d.z * in_matrix_3x3.m32,
            in_3d.x * in_matrix_3x3.m13 + in_3d.y * in_matrix_3x3.m23 + in_3d.z * in_matrix_3x3.m33);
    }

    fvector_4d mul(const fvector_4d& in_3d, const fmatrix_4x4& in_matrix_3x3)
    {
        return fvector_4d(
            in_3d.x * in_matrix_3x3.m11 + 
            in_3d.y * in_matrix_3x3.m21 +
            in_3d.z * in_matrix_3x3.m31 +
            in_3d.w * in_matrix_3x3.m41,

            in_3d.x * in_matrix_3x3.m12+
            in_3d.y * in_matrix_3x3.m22+
            in_3d.z * in_matrix_3x3.m32+
            in_3d.w * in_matrix_3x3.m42 ,
            
            in_3d.x * in_matrix_3x3.m13+
            in_3d.y * in_matrix_3x3.m23+
            in_3d.z * in_matrix_3x3.m33+
            in_3d.w * in_matrix_3x3.m43 ,

            in_3d.x * in_matrix_3x3.m14+
            in_3d.y * in_matrix_3x3.m24+
            in_3d.z * in_matrix_3x3.m34+
            in_3d.w * in_matrix_3x3.m44);
    }

    void rot_radian(float in_radian, fmatrix_3x3& in_world_matrix_3x3)
    {
        //旋转矩阵
        fmatrix_3x3 rot_matrix_3x3;

        //公式
        rot_matrix_3x3.m11 = cos(in_radian);
        rot_matrix_3x3.m12 = -sin(in_radian);
        rot_matrix_3x3.m21 = sin(in_radian);
        rot_matrix_3x3.m22 = cos(in_radian);

        //矩阵乘法
        in_world_matrix_3x3 = rot_matrix_3x3 * in_world_matrix_3x3;
    }

    void rot_angle(float in_angle, fmatrix_3x3& in_world_matrix_3x3)
    {
        float in_radian = in_angle * (3.1415926 / 180.0);

        rot_radian(in_radian, in_world_matrix_3x3);
    }

    void set_scale(const fvector_2d& in_scale, fmatrix_3x3& in_world_matrix_3x3)
    {
        in_world_matrix_3x3.m11 = in_scale.x;
        in_world_matrix_3x3.m22 = in_scale.y;
    }

    fmatrix_3x3 set_scale(const fvector_2d& in_scale)
    {
        fmatrix_3x3 in_world_matrix_3x3;
        set_scale(in_scale, in_world_matrix_3x3);

        return in_world_matrix_3x3;
    }

    bool scalar_neare_qual_float(float f1, float f2, float epsilon)
    {
        return fabsf(f1 - f2) <= epsilon;
    }

    fmatrix_4x4 matrix_perspective_base_r(float in_fov_radian, float aspect_ratio, float near_z, float far_z)
    {
        //作为检测是否合理
        assert(near_z > 0.f && far_z > 0.f);
        assert(!scalar_neare_qual_float(in_fov_radian,0.0f,0.00001f * 2.f));
        assert(!scalar_neare_qual_float(aspect_ratio, 0.0f, 0.00001f));
        assert(!scalar_neare_qual_float(near_z, far_z, 0.00001f));
        
        //构建透视矩阵
        fmatrix_4x4 matrix_4x4;
        {
            float d = near_z - far_z;
            float yscale = cos(in_fov_radian) / sin(in_fov_radian);
            float xscale = yscale / aspect_ratio;

            matrix_4x4.m11 = xscale;
            matrix_4x4.m22 = yscale;
            matrix_4x4.m33 = (near_z - far_z) / d;
            matrix_4x4.m34 = -1.f;
            matrix_4x4.m43 = (2.f * near_z * far_z) / d;
        }

        return matrix_4x4;
    }
}