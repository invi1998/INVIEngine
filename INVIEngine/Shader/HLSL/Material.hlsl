#include "ShaderCommon.hlsl"
#include "ShaderFunctionLibrary.hlsl"

// 定义材质
struct FMaterial
{
    // 通用材质
    float4 BaseColor;
};


// 菲尼尔
float3 FresnelSchlick(float3 F0, float3 PointNormal, float3 Direction, int M)
{
    return F0 + (1.f-F0) * pow(1.f - saturate(dot(PointNormal, Direction)), M);
}

// 获取basecolor
float4 GetMaterialBaseColor(MaterialConstBuffer MatConstbuffer, float2 Texcoord)
{
	if (MatConstbuffer.BaseColorIndex == -1)
	{
		return MatConstbuffer.BaseColor;
	}

	// 纹理采样 (v传入采样方式，传入UV）
	return SimpleTexture2DMap[MatConstbuffer.BaseColorIndex].Sample(SimplerTextureState, Texcoord);
}

// 获取法线贴图
float3 GetMaterialNormal(MaterialConstBuffer MatConstbuffer, float2 Texcoord, float3 unitWorldNormal, float3 worldTangent)
{
	if (MatConstbuffer.NormalIndex != -1)
	{
		float4 simpleNormal = SimpleTexture2DMap[MatConstbuffer.NormalIndex].Sample(SimplerTextureState, Texcoord);

		// 将 【0，1】的simpleNormal值 映射到 【-1， 1】
		float3 normalTangentSpace = (simpleNormal * 2.f - 1.f).rgb;

		// 拿到世界TBN
		float3x3 tbn = GetTBNMatrix(unitWorldNormal, worldTangent);

		// 将切线空间下的采样法线转为世界法线
		return mul(normalTangentSpace, tbn);

	}

	return unitWorldNormal;
}
