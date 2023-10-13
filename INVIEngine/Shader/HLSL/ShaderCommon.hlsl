#include "Light.hlsl"

// ����״̬
SamplerState SimplerTextureState : register(s0);

// ���� CBV������
//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV; // ��������������������Χ ��������Χ��Descriptor Range���Ĵ���
//	DescriptorRangeTextureSRV.Init(
//		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,	// ָ����ͼ������ָ������������ͼ �����������ͣ���������������������ѡ��shaderRenderingView
//		1,									// �������� 1
//		4);						// �����Ǹ���ɫ���ļĴ������󶨼Ĵ�����shaderRegister �� registerSpace����

// ��ͼ (����Ĵ����ı�Ŷ�Ӧ���ڸ�ǩ���������õ������CBV������ļĴ������
Texture2D SimpleTexture2DMap[TEXTURE2DNUM] : register(t3);

cbuffer MeshConstBuffer : register(b0)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	float4x4 MaterialTransformationMatrix; // ���ʱ任����
	float4x4 TextureTransformationMatrix; // ����任����
	
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

struct MaterialConstBuffer
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	int MaterialType; // ��������
	float MaterialRoughness; // ���ʴֲڶ�
	int BaseColorIndex; // ������ͼ������Ĭ��Ϊ-1����ʾû����ͼ��
	int NormalIndex;	// ������ͼ����
    
	float4 BaseColor; // ���ʻ�����ɫ
	float4x4 MaterialProjectionMatrix;
};

StructuredBuffer<MaterialConstBuffer> Materials : register(t4, space1);
