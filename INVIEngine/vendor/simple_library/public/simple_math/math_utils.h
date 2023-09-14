#pragma once
#include <vector>

//向量
#include "transformation/vector/vector_2d.h"
#include "transformation/vector/vector_3d.h"
#include "transformation/vector/vector_4d.h"

//矩阵
#include "transformation/matrix/matrix_2x2.h"
#include "transformation/matrix/matrix_3x3.h"
#include "transformation/matrix/matrix_4x4.h"

using namespace std;
namespace math_utils
{
	//矩阵乘法
	fvector_3d mul(const fvector_2d &in_3d, const fmatrix_3x3& in_matrix_3x3);
	fvector_3d mul(const fvector_3d &in_3d, const fmatrix_3x3& in_matrix_3x3);
	fvector_4d mul(const fvector_4d& in_3d, const fmatrix_4x4& in_matrix_3x3);

	//矩阵旋转
	void rot_radian(float in_radian, fmatrix_3x3& in_world_matrix_3x3);
	void rot_angle(float in_angle, fmatrix_3x3& in_world_matrix_3x3);

	//矩阵缩放
	void set_scale(const fvector_2d& in_scale, fmatrix_3x3& in_world_matrix_3x3);
	fmatrix_3x3 set_scale(const fvector_2d& in_scale);

	//判定bool是不是在一定范围内
	bool scalar_neare_qual_float(float f1,float f2,float epsilon);

	//基于右手的透视矩阵
	fmatrix_4x4 matrix_perspective_base_r(float in_fov_radian,float aspect_ratio,float near_z,float far_z);
}