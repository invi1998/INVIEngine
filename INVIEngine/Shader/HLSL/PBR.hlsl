#ifndef __SHADER_PBR__
#define __SHADER_PBR__

// pbr 渲染函数

// D 项 法线分布函数
float4 GetDistributionGGX(float3 N, float H, float Roughness)
{
    float PI = 3.1415926f;
    
    float a2 = pow(Roughness, 4);
    
    float NoH = saturate(dot(N, H));
    
    float d = (NoH * NoH * (a2 - 1) + 1);
    
    return a2 / max((d * d * PI), 0.0001f);
    
}

// G 项 几何方程
float SchlickGGX(float NoV, float Roughness)
{
    float K_Dir = pow(Roughness, 2) / 0.8f;
    
    return NoV / max(K_Dir + ((1 - K_Dir) * NoV), 0.0001f);
    
}

float GSmith(float3 N, float3 V, float3 L, float Roughness)
{
    float NoV = saturate(dot(N, V));
    float NoL = saturate(dot(N, L));
    
    return SchlickGGX(NoV, Roughness) * SchlickGGX(NoL, Roughness);
    
}

// 兰伯特 项
float3 GetDiffuseLambert(float3 DiffuseColor)
{
    float PI = 3.1415926f;
    return DiffuseColor * (1 / PI);
}

#endif
