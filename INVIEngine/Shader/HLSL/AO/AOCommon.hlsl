#ifndef __AO_COMMON__
#define __AO_COMMON__

#ifndef SAMPLE_VOLUME_NUM		// ���������������������ⲿ���룬���û�д�����Ĭ��Ϊ14�������㣬��Ϊ���������SSAO������������14��
#define SAMPLE_VOLUME_NUM 14
#endif

// ����״̬
SamplerState TextureSampler : register(s0); // ��ͼ������
SamplerState AnisotropicSampler : register(s1); // �������Բ�����
SamplerComparisonState ShadowSampler : register(s2); // ��Ӱ������
SamplerState DepthSampler : register(s3); // �����Ϣ������

Texture2D SampleNormalMap : register(t0); // ����
Texture2D SampleDepthMap : register(t1); // ���
Texture2D SampleNoiseMap : register(t2); // �벨
Texture2D SampleAcceptMap : register(t3); // �������ܣ�������������SSAO�벨ͼ����˫���˲���ͼ

static const float2 TextureCoordinates[6] =
{
	float2(0.f, 1.f),
	float2(0.f, 0.f),
	float2(1.f, 0.f),
	
	float2(0.f, 1.f),
	float2(1.f, 0.f),
	float2(1.f, 1.f),
};

struct MeshVertexOut
{
	float4 ViewPosition : POSITION;
	float4 Position : SV_POSITION;
	float2 Texcoord : TEXCOORD; // UV
};

// SSAO��������
cbuffer CBufferSSAOView : register(b0)
{
	float4x4 InversiveProjectionMatrix; // ������ͶӰ����
	float4x4 ProjectionMatrix; // �������ͶӰ����
	float4x4 TextureProjectionMatrix; // ����ռ��ͶӰ����
	
	float OcclusionRadius; // �ڱΰ뾶
	float OcclusionFadeStart; // �ڱε�����ʼ
	float OcclusionFadeEnd; // �ڱε�������
	float ObscurationThreshold; // �ڱ���ֵ
	
	float4 SampleVolumeData[SAMPLE_VOLUME_NUM]; // ����������
}

// ģ������
cbuffer CBufferBlurConstants : register(b1)
{
	bool BlurParam1; // ģ������
}

// ģ�����ӣ���CPU���岢����
cbuffer CBufferSSAOBlurParam : register(b2)
{
	// ģ��Ȩ��
	float4 BlurWeight[3];
}

#endif
