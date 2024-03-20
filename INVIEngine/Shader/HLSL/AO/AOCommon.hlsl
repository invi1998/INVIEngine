#ifndef __AO_COMMON__
#define __AO_COMMON__

#ifndef SAMPLE_VOLUME_NUM		// 这个采样点的数量可以由外部传入，如果没有传入则默认为14个采样点，因为我们引擎的SSAO采样点数量是14个
#define SAMPLE_VOLUME_NUM 14
#endif

// 采样状态
SamplerState TextureSampler : register(s0); // 贴图采样器
SamplerState AnisotropicSampler : register(s1); // 各向异性采样器
SamplerComparisonState ShadowSampler : register(s2); // 阴影采样器
SamplerState DepthSampler : register(s3); // 深度信息采样器

Texture2D SampleNormalMap : register(t0); // 法线
Texture2D SampleDepthMap : register(t1); // 深度
Texture2D SampleNoiseMap : register(t2); // 噪波
Texture2D SampleAcceptMap : register(t3); // 采样接受，传入的是随机的SSAO噪波图还是双边滤波的图

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

// SSAO采样参数
cbuffer CBufferSSAOView : register(b0)
{
	float4x4 InversiveProjectionMatrix; // 求逆后的投影矩阵
	float4x4 ProjectionMatrix; // 摄像机的投影矩阵
	float4x4 TextureProjectionMatrix; // 纹理空间的投影矩阵
	
	float OcclusionRadius; // 遮蔽半径
	float OcclusionFadeStart; // 遮蔽淡出开始
	float OcclusionFadeEnd; // 遮蔽淡出结束
	float ObscurationThreshold; // 遮蔽阈值
	
	float4 SampleVolumeData[SAMPLE_VOLUME_NUM]; // 采样体数据
}

// 模糊参数
cbuffer CBufferBlurConstants : register(b1)
{
	bool BlurParam1; // 模糊参数
}

// 模糊算子，由CPU定义并传入
cbuffer CBufferSSAOBlurParam : register(b2)
{
	// 模糊权重
	float4 BlurWeight[3];
}

#endif
