#ifndef __SHADER__COMMON__
#define __SHADER__COMMON__

#include "Light.hlsl"

// ����״̬
SamplerState TextureSampler : register(s0);		// ��ͼ������
SamplerState AnisotropicSampler : register(s1);		// �������Բ�����
SamplerComparisonState ShadowSampler : register(s2);		// ��Ӱ������
SamplerState DepthSampler : register(s3); // �����Ϣ������

// ���� CBV������
//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV; // ��������������������Χ ��������Χ��Descriptor Range���Ĵ���
//	DescriptorRangeTextureSRV.Init(
//		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,	// ָ����ͼ������ָ������������ͼ �����������ͣ���������������������ѡ��shaderRenderingView
//		1,									// �������� 1
//		4);						// �����Ǹ���ɫ���ļĴ������󶨼Ĵ�����shaderRegister �� registerSpace����

// ��ͼ (����Ĵ����ı�Ŷ�Ӧ���ڸ�ǩ���������õ������CBV������ļĴ������

TextureCube SimpleCubeMap : register(t0); // ��պ�
TextureCube SimpleShadowCubeMap : register(t1); // ������Ӱ��ͼ ���Դ
Texture2D SimpleShadowMap : register(t2);
Texture2D SimpleSSAOMap : register(t3);
Texture2D SimpleTexture2DMap[TEXTURE2D_MAP_NUM] : register(t4);		// Ϊ�˱��������ͼ���ݶ��䵼�µ���Դ�������⣬�����������͵����ݾ����������ļĴ�����

cbuffer MeshConstBuffer : register(b0)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	float4x4 WorldMatrix; // ����任����
	float4x4 TextureTransformationMatrix; // ����任����
	float4x4 NormalMatrix; // ���߱任���� �����ȱ�����ʱ�ķ��߱任
	
	uint MaterialID; // ����id
	uint rr1;
	uint rr2;
	uint rr3;
}


cbuffer ViewportConstBuffer : register(b1)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	float4 CameraPosition;
	// �ӿ�ͶӰ����
	float4x4 ViewportProjectionMatrix;
    
}

cbuffer LightConstBuffer : register(b2)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	Light SceneLights[16]; // �����ƹ�
}


// ��
cbuffer FogConstBuffer : register(b3)
{
	float4 FogColor;

	float FogStart;
	float FogRange;
	float FogHeight;
	float FogTransparentCoefficient; // ���͸��ϵ��
}

struct MaterialConstBuffer
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	int MaterialType; // ��������
	float MaterialRoughness; // ���ʴֲڶ�
	int BaseColorIndex; // ������ͼ������Ĭ��Ϊ-1����ʾû����ͼ��
	int NormalIndex;	// ������ͼ����

	int SpecularIndex;
	float Param0;		// �Զ������
	float Param1;
	float Param2;
    
	float4 BaseColor; // ���ʻ�����ɫ

	float3 SpecularColor;
	float RefractiveValue;	// ������
	
	float3 FresnelF0;
	float Transparency;

	float4x4 MaterialProjectionMatrix;
	
	float3 Metallicity;		// ������
	float XX4;
};

StructuredBuffer<MaterialConstBuffer> Materials : register(t0, space1);

#endif
