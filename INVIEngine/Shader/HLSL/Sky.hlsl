#include "ShaderCommon.hlsl"
#include "SkyFunction.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // λ��
	float3 Normal : NORMAL; // ����
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 PositionH : POSITION;
	float4 PositionL : SV_POSITION;
};


// ������ɫ��
MeshVertexOut VertexShaderMain(MeshVertexIn mvIn)
{
	MeshVertexOut Out = (MeshVertexOut) 0;

	Out.PositionH = float4(mvIn.Position, 1.f);

	// ��������
	float4 WorldPosition = mul(Out.PositionH, WorldMatrix);

	// �任����βü��ռ�
	Out.PositionL = mul(WorldPosition, ViewportProjectionMatrix);

	return Out;

}


// ������ɫ��
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_TARGET
{
	// ������
	// mvOut.Color = GetFogValue(mvOut.Color);
	float4 WorldPosition = mul(mvOut.PositionH, WorldMatrix);
	
	float4 Color = SimpleCubeMap.Sample(TextureSampler, mvOut.PositionH.xyz);
	
	Color = GetFogValue(Color, WorldPosition.xyz);
	return Color;
	// return float4(0.3f, 0.24f, 0.12f, 1.0f);
}
