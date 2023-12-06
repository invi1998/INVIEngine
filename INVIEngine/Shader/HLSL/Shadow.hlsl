#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // Œª÷√
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 PositionH : POSITIONT;
	float2 Texcoord : TEXCOORD;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];
	
	MeshVertexOut Out = (MeshVertexOut) 0;
	
	float4 PositionWorld = mul(float4(mv.Position, 1.f), MaterialTransformationMatrix);

}


void PSMain(MeshVertexOut mvOut)
{
	
}

