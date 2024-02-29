// 操作物体的shader

#include "Material.hlsl"
#include "ShadowFunction.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // 位置
	float2 TexCoord : TEXCOORD;	// UV
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 PositionH : SV_POSITION;
	float2 TexCoord : TEXCOORD; // UV
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];
	
	MeshVertexOut Out = (MeshVertexOut) 0;
	
	// 将模型的顶点坐标转为世界坐标
	Out.WorldPosition = mul(float4(mv.Position, 1.f), WorldMatrix);
	
	// 将世界坐标转为视口裁剪空间坐标
	Out.PositionH = mul(Out.WorldPosition, ViewportProjectionMatrix);
	
	// UV坐标
	float4 TextureTexcoord = mul(float4(mv.TexCoord, 0.f, 1.f), TextureTransformationMatrix);
	Out.TexCoord = mul(TextureTexcoord, MatConstBuffer.MaterialProjectionMatrix).xy;
	
	return Out;

}


float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];
	
	float r = SampleBuildCDTexture(mvOut.TexCoord);

	return float4(r, r, r, 0.35f);
}


