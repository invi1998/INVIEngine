#include "AOFunc.hlsl"

struct MeshVertexOutBlur
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

MeshVertexOutBlur VSMain(uint VertexID : SV_VertexID)
{
	MeshVertexOutBlur Out = (MeshVertexOutBlur) 0.f;

	Out.TexCoord = TextureCoordinates[VertexID];

	//0-1  NDC空间 
	Out.Position = float4(2.f * Out.TexCoord.x - 1.f, 1.f - 2.f * Out.TexCoord.y, 0.f, 1.f);

	return Out;
}

float4 PSMain(MeshVertexOutBlur mvOut) : SV_TARGET
{
	float BlurWeight[12] =
	{
		Float4BlurWeights[0].x, Float4BlurWeights[0].y, Float4BlurWeights[0].z, Float4BlurWeights[0].w,
		Float4BlurWeights[1].x, Float4BlurWeights[1].y, Float4BlurWeights[1].z, Float4BlurWeights[1].w,
		Float4BlurWeights[2].x, Float4BlurWeights[2].y, Float4BlurWeights[2].z, Float4BlurWeights[2].w,
	};
	
	float2 TexOffset;
	if (bHorizontalBlur)
	{
		TexOffset = float2(1.f / InversionTextureSize.x, 0.f);	// 像素在x轴上的偏移
	}
	else
	{
		TexOffset = float2(0.f, 1.f / InversionTextureSize.y);	// 像素在y轴上的偏移
	}

	// 中心权重对应的颜色，法线，深度
	float weightSum = BlurWeight[BlurRadius];
	float4 Color = weightSum * SampleAcceptMap.SampleLevel(TextureSampler, mvOut.TexCoord, 0);		// 获取采样接受图的颜色

	float3 Normal = SampleNormalMap.SampleLevel(TextureSampler, mvOut.TexCoord, 0).rgb;		// 采样法线

	float NDCDepth = SampleDepthMap.SampleLevel(DepthSampler, mvOut.TexCoord, 0).r;		// 采样深度
	float Depth = DepthNDCToView(NDCDepth);		// NDC深度转换为视图空间深度

	for (float i = -BlurRadius; i <= BlurRadius; ++i)
	{
		if (i == 0)
		{
			continue;	// 跳过中心像素
		}

		float2 uv = mvOut.TexCoord + i * TexOffset;

		float3 blurNormal = SampleNormalMap.SampleLevel(TextureSampler, uv, 0).rgb;		// 采样法线
		float blurNDCDepth = SampleDepthMap.SampleLevel(DepthSampler, uv, 0).r;		// 采样深度
		float blurDepth = DepthNDCToView(blurNDCDepth);

		if (abs(Depth - blurDepth) > 0.1f)
		{
			continue;	// 深度差异过大，跳过
		}
		if (dot(Normal, blurNormal) < 0.8f)
		{
			continue;	// 法线差异过大，跳过
		}

		float weight = BlurWeight[BlurRadius + i];
		Color += weight * SampleAcceptMap.SampleLevel(TextureSampler, uv, 0);

		weightSum += weight;

	}

	// float3 tempColor = SampleAcceptMap.SampleLevel(TextureSampler, mvOut.TexCoord, 0).rgb;
	return Color / weightSum;
}
