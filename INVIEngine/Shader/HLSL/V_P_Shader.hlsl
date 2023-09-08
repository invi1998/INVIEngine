
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

//float2 tri(in float2 x)
//{
//    float2 h = frac(x * 0.5f) - 0.5f;
//    return 1.f - 2.f * abs(h);
//}

//float checkersGrid(float2 uv, float2 ddx, float2 ddy)
//{
//    float2 w = max(abs(ddx), abs(ddy)) + 0.01f;
//    float2 i = (tri(uv + 0.5 * w) - tri(uv - 0.5 * w)) / w;
//    return 0.5 - 0.5 * i.x * i.y;
//}


MeshVertexOut VSMain(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// 获取顶点的世界坐标
    outV.WorldPosition = mul(float4(mv.Position, 1.0f), WorldMatrix);
    
    // 将模型转到其次裁剪空间
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
    // material.BaseColor = float4(0.82f, 0.82f, 0.82f, 1.0f);
    
    float DotDiffValue = 0.f;
    
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
        // reflect hlsl中用于求光线的反射光的函数，可以用来计算反射向量
        float3 ReflectDirection = normalize(-reflect(NormalizeLightDirection, ModelNormal));
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
        float3 HalfDirection = normalize(NormalizeLightDirection + ViewDirection);
    
        // 计算出Blinn-phong值
        DotDiffValue = max(0.0f, dot(ModelNormal, HalfDirection));
        
        if (DotDiffValue > 0.f)
        {
            float MaterialShiniess = 1.f - saturate(MaterialRoughness);
            float M = MaterialShiniess * 100.f;
            
            Specular = pow(max(dot(HalfDirection, ModelNormal), 0.f), M);
        }

    }
    else if (MaterialType == 4)
    {
        // WrapLight模型 早期皮肤模拟
        
        // float WrapValue = 1.f;  // 该值为1的时候，是半兰伯特材质
        float WrapValue = 2.5f;    // 该值越高，皮肤效果越通透
        float DiffueseReflection = dot(ModelNormal, NormalizeLightDirection);
        DotDiffValue = max(0.0f, (DiffueseReflection + WrapValue) / (1.f + WrapValue)); // [-1, 1]->[0.1]
        
    }
    else if (MaterialType == 5)
    {
        // Minnaert模型（月光模型）
        
        // 第一种 dot(l, n) * dot(n, v)
        
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        float DotLight = max(dot(ModelNormal, NormalizeLightDirection), 0.f);
        float DotView = max(dot(ModelNormal, ViewDirection), 0.f);
        
        // DotDiffValue = DotLight * DotView;
        
        // 第二种 dot(l, n) * pow(dot(l, n) * dot(n, v), r)
        
        float MaterialShiniess = 1.f - saturate(MaterialRoughness);
        float M = MaterialShiniess * 10.f;
        
        DotDiffValue = saturate(DotLight * pow(DotLight * DotView, M));
        
        
    }
    else if (MaterialType == 6)
    {
        // Banded 基础卡通
        
        float DiffueseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;
        
        // 分层数量
        float Layered = 4.f;
        
        DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
        
    }
    else if (MaterialType == 7)
    {
        // GradualBanded  带渐变的卡通效果
        
        // 渐变颜色
        float4 GradualColor = { 0.87f, 0.12f, 0.6f, 1.f };
        
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        float LightDotValue = dot(ModelNormal, NormalizeLightDirection);
        
        float DiffueseReflection = (LightDotValue + 1.f) * 0.5f;
        
        // 分层数量
        float Layered = 8.f;
        
        DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
        
        material.BaseColor = lerp(material.BaseColor, GradualColor, LightDotValue);
        
    }
    else if (MaterialType == 8)
    {
        // CustomBanded  自定义卡通效果
        
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        float DiffueseReflection = (dot(ModelNormal, NormalizeLightDirection) + 1.f) * 0.5f;
        
        // 分层数量
        float Layered = 4.f;
        
        DotDiffValue = floor(DiffueseReflection * Layered) / Layered;
        
        // 加入菲尼尔效果
        float3 f0 = { 0.02f, 0.02f, 0.02f };
        Specular.xyz = FresnelSchlick(f0, ModelNormal, ViewDirection, 2);
        
        // 获取光线和摄像机视角的半程向量
        float3 HalfDirection = normalize(NormalizeLightDirection + ViewDirection);
        
        // 加入高光
        if (DotDiffValue > 0.f)
        {
            float MaterialShiniess = 1.f - saturate(MaterialRoughness);
            float M = MaterialShiniess * 70.f;
            
            Specular += pow(max(dot(HalfDirection, ModelNormal), 0.f), M) / 0.032f;
        }
        
    }
    else if (MaterialType == 100)
    {
        // 菲尼尔
        float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
        
        float3 f0 = { 0.02f, 0.02f, 0.02f };
        Specular.xyz = FresnelSchlick(f0, ModelNormal, ViewDirection, 2);
    }
 
    // 最终颜色贡献
    // material.BaseColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    mvOut.Color = material.BaseColor * DotDiffValue // 漫反射
    + material.BaseColor * AmbientLight // 间接光（环境光）
    + material.BaseColor * Specular; // 高光
    
	// 伽马校正
    // mvOut.Color = sqrt(mvOut.Color);
    
    return mvOut.Color;
    
    // return CameraPosition;

    // return BaseColor;

}
