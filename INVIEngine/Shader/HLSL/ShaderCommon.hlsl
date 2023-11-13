#include "Light.hlsl"

// 采样状态
SamplerState TextureSampler : register(s0);
SamplerState AnisotropicSampler : register(s1);

// 纹理 CBV描述表
//CD3DX12_DESCRIPTOR_RANGE DescriptorRangeTextureSRV; // 常量缓冲区区描述符范围 描述符范围（Descriptor Range）的创建
//	DescriptorRangeTextureSRV.Init(
//		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,	// 指定视图（这里指向常量缓冲区视图 （描述符类型）），对于纹理，这里我们选择shaderRenderingView
//		1,									// 描述数量 1
//		4);						// 基于那个着色器的寄存器（绑定寄存器（shaderRegister 和 registerSpace））

// 贴图 (这里寄存器的编号对应你在根签名那里设置的纹理的CBV描述表的寄存器编号
Texture2D SimpleTexture2DMap[TEXTURE2DNUM] : register(t1);
// TextureCube SimpleCubeMap[CUBE_MAP_NUM] : register(t5, space1);	// 天空盒

cbuffer MeshConstBuffer : register(b0)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	float4x4 MaterialTransformationMatrix; // 材质变换矩阵
	float4x4 TextureTransformationMatrix; // 纹理变换矩阵
	
	uint MaterialID; // 材质id
	uint rr1;
	uint rr2;
	uint rr3;
}


cbuffer ViewportConstBuffer : register(b1)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	float4 CameraPosition;
	// 视口投影矩阵
	float4x4 ViewportProjectionMatrix;
    
}

cbuffer LightConstBuffer : register(b2)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	Light SceneLights[16]; // 场景灯光
}

struct MaterialConstBuffer
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	int MaterialType; // 材质类型
	float MaterialRoughness; // 材质粗糙度
	int BaseColorIndex; // 纹理贴图索引（默认为-1，表示没有贴图）
	int NormalIndex;	// 法线贴图索引

	int SpecularIndex;
	int XX1;
	int XX2;
	int XX3;
    
	float4 BaseColor; // 材质基础颜色

	float3 SpecularColor;
	int XX6;

	float4x4 MaterialProjectionMatrix;
};

StructuredBuffer<MaterialConstBuffer> Materials : register(t0, space1);
