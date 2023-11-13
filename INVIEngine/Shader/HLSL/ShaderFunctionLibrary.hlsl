#pragma once

/**
 * \brief ����TBN����
 * \param unitWorldNormal  ��λ�������編��
 * \param worldTangent ��������
 * \return ����3x3�ľ���
 */
float3x3 GetTBNMatrix(float3 unitWorldNormal, float3 worldTangent)
{
	float3 N = unitWorldNormal;
	// T ���ߵ����������� ��ȥ (�������������編���ϵ�ͶӰ * ���編��)
	// ��ʵ��һ���ļ������壬����Ϊ�˵õ�һ����ֱ�뷨�ߵ�����T��������ǵ����ߺ����編�߲���ֱ����ô��һ�����Զ��������ݽ���У�����õ�һ����ֱ�����編�ߵ�����T
	float3 T = normalize(worldTangent - dot(worldTangent, N) * N);
	float3 B = cross(N, T);

	return float3x3(T, B, N);
}
