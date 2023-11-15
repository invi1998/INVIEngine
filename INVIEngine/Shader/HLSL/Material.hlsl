#ifndef __SHADER_MATERIAL__
#define __SHADER_MATERIAL__


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
	return SimpleTexture2DMap[MatConstbuffer.BaseColorIndex].Sample(TextureSampler, Texcoord);
}

// 获取法线贴图
float3 GetMaterialNormal(MaterialConstBuffer MatConstbuffer, float2 Texcoord, float3 unitWorldNormal, float3 worldTangent)
{
	if (MatConstbuffer.NormalIndex != -1)
	{
		float4 simpleNormal = SimpleTexture2DMap[MatConstbuffer.NormalIndex].Sample(AnisotropicSampler, Texcoord);

		// 将 【0，1】的simpleNormal值 映射到 【-1， 1】
		float3 normalTangentSpace = 2.f * simpleNormal.rgb - 1.f;

		// 拿到世界TBN
		float3x3 tbn = GetTBNMatrix(unitWorldNormal, worldTangent);

		// 将切线空间下的采样法线转为世界法线
		return mul(normalTangentSpace, tbn);

	}

	return unitWorldNormal;
}

// 获取高光贴图
float4 GetMaterialSpecular(MaterialConstBuffer MatConstbuffer, float2 Texcoord)
{
	if (MatConstbuffer.SpecularIndex == -1)
	{
		return float4(MatConstbuffer.SpecularColor, 1.0f);
	}

	// 纹理采样 (v传入采样方式，传入UV）
	return SimpleTexture2DMap[MatConstbuffer.SpecularIndex].Sample(TextureSampler, Texcoord);
}

// 获取反射（视角向量和顶点世界法线的反射向量）
float3 GetReflect(float3 UnitWorldNormal)
{
	return reflect(-CameraPosition, UnitWorldNormal);
}

// 获取反射采样颜色
float3 GetReflectionSampleColor(float3 UnitWorldNormal, float3 NewReflect)
{
	return SimpleCubeMap[0].Sample(TextureSampler, NewReflect).rgb;
}

// 获取反射度(决定当前反射颜色的强度）
float GetShininess(MaterialConstBuffer MaterialBuff)
{
	// 1 - 材质粗糙度
	return 1.f - MaterialBuff.MaterialRoughness;
}

// 获取菲尼尔因子
float3 FresnelSchlickFactor(MaterialConstBuffer MaterialBuff, float3 UnitWorldNormal, float3 inReflect)
{
	return FresnelSchlick(MaterialBuff.FresnelF0, UnitWorldNormal, inReflect, 5);
}

// 最终获取的反射颜色
float3 GetReflectionColor(MaterialConstBuffer MaterialBuff, float3 UnitWorldNormal)
{
	float3 NewReflect = GetReflect(UnitWorldNormal);

	// 反射采样颜色
	float3 ReflectSampleColor = GetReflectionSampleColor(UnitWorldNorma, NewReflect);

	// 粗糙度
	float3 shiness = GetShininess(MaterialBuff);

	// 菲尼尔
	float3 Fresnel = FresnelSchlickFactor(MaterialBuff, UnitWorldNormal, NewReflect);

	return saturate(ReflectSampleColor * shiness * Fresnel);

}

#endif
