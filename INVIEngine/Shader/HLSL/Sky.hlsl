#include "ShaderCommon.hlsl"

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

	Out.PositionL = float4(mvIn.Position, 1.f);

	// 世界坐标
	float4 WorldPosition = mul(Out.PositionL, MaterialTransformationMatrix);

	// 变换到其次裁剪空间
	Out.PositionL = mul(WorldPosition, ViewportProjectionMatrix);

	return Out;

}


// 像素着色器
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_TARGET
{
	// return SimpleCubeMap[0].Sample(TextureSampler, mvOut.PositionL);
	return float4(0.3f, 0.24f, 0.12f, 1.0f);
}
