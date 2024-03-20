#pragma once

struct FSSAOConstant
{
	FSSAOConstant();

	XMFLOAT4X4 InversiveProjectionMatrix{};	// ������ͶӰ����
	XMFLOAT4X4 ProjectionMatrix{};			// �������ͶӰ����
	XMFLOAT4X4 TextureProjectionMatrix{};		// ����ռ��ͶӰ����

	float OcclusionRadius = 0.5f; // �ڱΰ뾶
	float OcclusionFadeStart = 0.2f; // �ڱε�����ʼ
	float OcclusionFadeEnd = 1.f; // �ڱε�������
	float ObscurationThreshold = 0.05f; // �ڱ���

	XMFLOAT4 SampleVolumeData[14]{};	// 14���������
};

struct FSSAOBlurParam
{
	FSSAOBlurParam();

	XMFLOAT4 BlurWeight[3]{};	// 3��ģ��Ȩ��
};

