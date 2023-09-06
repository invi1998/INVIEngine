
#include "Light.hlsl"
#include "Material.hlsl"

cbuffer MeshConstBuffer : register(b0)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	float4x4 WorldMatrix;
}


cbuffer ViewportConstBuffer : register(b1)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
    float4 CameraPosition;
	// 视口投影矩阵
	float4x4 ViewportProjectionMatrix;
    
}

cbuffer MaterialConstBuffer : register(b2)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
    int MaterialType;       // 材质类型
    
    float4 BaseColor;       // 材质基础颜色
    float MaterialRoughness;    // 材质粗糙度
	float4x4 MaterialProjectionMatrix;
}

cbuffer LightConstBuffer : register(b3)
{
	// 声明常量缓冲区(我们需要将程序里的常量缓冲区的数据寄存到寄存器里，寄存器有15个b0-b14，然后从寄存器里读取出来使用)
	float3 LightIntensity;		// 光照强度
	float3 LightDirection;		// 光照方向
}

struct MeshVertexIn
{
	float3 Position : POSITION;		// 位置
	float4 Color : COLOR;			// 颜色
	float3 Normal : NORMAL;			// 法线
};

struct MeshVertexOut
{
    float4 WorldPosition : POSITION;
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

float2 tri(in float2 x)
{
    float2 h = frac(x * 0.5f) - 0.5f;
    return 1.f - 2.f * abs(h);
}

float checkersGrid(float2 uv, float2 ddx, float2 ddy)
{
    float2 w = max(abs(ddx), abs(ddy)) + 0.01f;
    float2 i = (tri(uv + 0.5 * w) - tri(uv - 0.5 * w)) / w;
    return 0.5 - 0.5 * i.x * i.y;
}


MeshVertexOut VSMain(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// 将模型转到其次裁剪空间
    outV.WorldPosition = mul(float4(mv.Position, 1.0f), WorldMatrix);
    outV.Position = mul(outV.WorldPosition, ViewportProjectionMatrix);

    outV.Normal = mul(mv.Normal, (float3x3) WorldMatrix);

    outV.Color = mv.Color;

	return outV;
}

//float3 render(float2 uv, in float2 px, in float2 py)
//{
//	// 平面
//    float3 rdx = normalize(ViewportProjectionMatrix * float3(px, fl));
//    float3 rdy = normalize(ViewportProjectionMatrix * float3(py, fl));

//    float3 ddx = ro.y * (rd / rd.y - rdx / rdx.y);
//    float3 ddy = ro.y * (rd / rd.y - rdy / rdy.y);

//    c = float3(0.) + float3(0.9) * checkersGrid(p.xz, ddx.xz, ddy.xz);
//}

float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
    // 环境光
    float4 AmbientLight = { 0.15f, 0.15f, 0.22f, 1.0f };

    float3 ModelNormal = normalize(mvOut.Normal);
    float3 NormalizeLightDirection = normalize(-LightDirection);
    
    FMaterial material;
    material.BaseColor = BaseColor;
    
    float DotDiffValue = max(dot(ModelNormal, NormalizeLightDirection), 0.0f);

    // DotDiffValue = DotDiffValue * 0.5f + 0.5f;
    
    float4 Specular = { 0.f, 0.f, 0.f, 1.f };
    
    if (MaterialType == 0)
    {
        // 兰伯特材质
        DotDiffValue = max(dot(ModelNormal, NormalizeLightDirection), 0.f);
    }
    else if (MaterialType == 1)
    {
        // 半兰伯特材质
        float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
        DotDiffValue = max(0.0f, (DotDiffValue * 0.5f + 0.5f));     // [-1, 1]->[0.1]
    }
    else if (MaterialType == 2)
    {
        // Phong
        float3 ReflectDirection = normalize(-reflect(ModelNormal, NormalizeLightDirection));
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        DotDiffValue = max(dot(ModelNormal, NormalizeLightDirection), 0.0f);
        
        if (DotDiffValue > 0.f)
        {
            float MaterialShiniess = 1.f - saturate(MaterialRoughness);
            float M = MaterialShiniess * 100.f;
            
            Specular = pow(max(dot(ViewDirection, ReflectDirection), 0.f), M);
        }
    }
    else if (MaterialType == 3)
    {
        // Blinn-Phong
        
        // 获取摄像机到像素点的向量
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        // 获取光线和摄像机视角的半程向量
        float3 HalfDirection = normalize(-LightDirection + ViewDirection);
    
        // 计算出Blinn-phong值
        DotDiffValue = max(0.0f, dot(mvOut.Normal, HalfDirection));
        
        if (DotDiffValue > 0.f)
        {
            float MaterialShiniess = 1.f - saturate(MaterialRoughness);
            float M = MaterialShiniess * 100.f;
            
            Specular = pow(max(dot(ViewDirection, ViewDirection), 0.f), M);
        }

    }
    
 
    // 最终颜色贡献
    mvOut.Color = material.BaseColor * DotDiffValue // 漫反射
    + AmbientLight * material.BaseColor  // 间接光（环境光）
    + Specular * material.BaseColor;    // 高光
    
	// 伽马校正
    mvOut.Color = sqrt(mvOut.Color);
    
    return mvOut.Color;

}
