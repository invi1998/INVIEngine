#ifndef __SHADER_PBR__
#define __SHADER_PBR__

// pbr 渲染函数

// D 项 法线分布函数
float4 GetDistributionGGX(float3 N, float3 H, float Roughness)
{
    float PI = 3.1415926f;
    
    float a2 = pow(Roughness, 2);
    
    float NoH = saturate(dot(N, H));
    
    float d = (NoH * NoH * (a2 - 1) + 1);
    
    return a2 / max((d * d * PI), 0.001f);
    
}

// G 项 几何方程
float SchlickGGX(float Dot, float K_Dir)
{
	return Dot / max(K_Dir + ((1 - K_Dir) * Dot), 0.001);
}

float GSmith(float3 N, float3 V, float3 L, float Roughness)
{
	float K_Dir = pow(Roughness, 2) / 8.f;

    float NoV = saturate(dot(N, V));
    float NoL = saturate(dot(N, L));
    
	return SchlickGGX(NoV, K_Dir) * SchlickGGX(NoL, K_Dir);
    
}

// 兰伯特 项
float3 GetDiffuseLambert(float3 DiffuseColor)
{
    float PI = 3.1415926f;
    return DiffuseColor * (1 / PI);
}

//间接光
float3 GetDirectLight(float HoL, float3 F0, float Roughness)
{
	float F = exp2((-5.55473 * HoL - 6.98316) * HoL);
	return lerp(F, 1, F0);
}

//直接光
float3 GetIndirectLight(float NoV, float3 F0, float Roughness)
{
	float F = exp2((-5.55473 * NoV - 6.98316) * NoV);
	return F0 + F * saturate(1 - Roughness - F0);
}

#endif
