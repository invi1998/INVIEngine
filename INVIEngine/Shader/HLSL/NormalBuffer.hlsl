#include "Material.hlsl"
#include "PBR.hlsl"
#include "SkyFunction.hlsl"
#include "ShadowFunction.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // 位置
	float3 Normal : NORMAL; // 法线
	float3 UTangent : TANGENT; // 切线（U方向）
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float3 UTangent : TANGENT; // 切线（U方向）
	float2 Texcoord : TEXCOORD; // UV
};


MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstbuffer = Materials[MaterialID];
	
	MeshVertexOut outV;

	// 获取顶点的世界坐标
	outV.WorldPosition = mul(float4(mv.Position, 1.0f), WorldMatrix);
    
    // 将模型转到其次裁剪空间
	outV.Position = mul(outV.WorldPosition, ViewportProjectionMatrix);
	
	// 法线（从模型空间转到世界空间）
	outV.Normal = mul(mv.Normal, (float3x3) NormalMatrix);

    // 切线 (从切线空间转到世界空间）
	outV.UTangent = mul(mv.UTangent, (float3x3) NormalMatrix);
    
    // UV
    // 先将传入的uv坐标和模型的纹理变换相乘，得到纹理变换后的UV
	float4 TextureTexcoord = mul(float4(mv.Texcoord, 0.f, 1.f), TextureTransformationMatrix);
    // 然后再与材质的变换矩阵相乘，得到材质变换后的UV，就是我们最终的顶点UV了
	outV.Texcoord = mul(TextureTexcoord, MatConstbuffer.MaterialProjectionMatrix).xy;

	return outV;
}

float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	float3 NormallizedWorldNormal = normalize(mvOut.Normal);
	
	// 将法线转到视口空间
	float3 ViewNormal = mul(NormallizedWorldNormal, (float3x3)ViewportProjectionMatrix);
	
	return float4(ViewNormal, 0.0f);
}
