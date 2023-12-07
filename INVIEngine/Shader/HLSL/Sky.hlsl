#include "ShaderCommon.hlsl"
#include "SkyFunction.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // 位置
	float3 Normal : NORMAL; // 法线
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 PositionH : POSITION;
	float4 PositionL : SV_POSITION;
};


// 顶点着色器
MeshVertexOut VertexShaderMain(MeshVertexIn mvIn)
{
	MeshVertexOut Out = (MeshVertexOut) 0;

	Out.PositionH = float4(mvIn.Position, 1.f);

	// 世界坐标
	float4 WorldPosition = mul(Out.PositionH, WorldMatrix);

	// 变换到其次裁剪空间
	Out.PositionL = mul(WorldPosition, ViewportProjectionMatrix);

	return Out;

}


// 像素着色器
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_TARGET
{
	// 计算雾
	// mvOut.Color = GetFogValue(mvOut.Color);
	float4 WorldPosition = mul(mvOut.PositionH, WorldMatrix);
	
	float4 Color = SimpleCubeMap.Sample(TextureSampler, mvOut.PositionH.xyz);
	
	Color = GetFogValue(Color, WorldPosition.xyz);
	return Color;
	// return float4(0.3f, 0.24f, 0.12f, 1.0f);
}
