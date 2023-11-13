#include "ShaderCommon.hlsl"

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

	Out.PositionL = float4(mvIn.Position, 1.f);

	// ��������
	float4 WorldPosition = mul(Out.PositionL, MaterialTransformationMatrix);

	// �任����βü��ռ�
	Out.PositionL = mul(WorldPosition, ViewportProjectionMatrix);

	return Out;

}


// ������ɫ��
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_TARGET
{
	// return SimpleCubeMap[0].Sample(TextureSampler, mvOut.PositionL);
	return float4(0.3f, 0.24f, 0.12f, 1.0f);
}
