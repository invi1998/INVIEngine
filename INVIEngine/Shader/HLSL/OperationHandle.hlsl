// 操作物体的shader

#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // 位置
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 PositionH : SV_POSITION;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	
	MeshVertexOut Out = (MeshVertexOut) 0;
	
	// 将模型的顶点坐标转为世界坐标
	Out.WorldPosition = mul(float4(mv.Position, 1.f), WorldMatrix);
	
	// 将世界坐标转为视口裁剪空间坐标
	Out.PositionH = mul(Out.WorldPosition, ViewportProjectionMatrix);
	
	return Out;

}


float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];

	return MatConstBuffer.BaseColor;
}


