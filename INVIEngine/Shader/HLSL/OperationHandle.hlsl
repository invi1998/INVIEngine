// ���������shader

#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // λ��
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 PositionH : SV_POSITION;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	
	MeshVertexOut Out = (MeshVertexOut) 0;
	
	// ��ģ�͵Ķ�������תΪ��������
	Out.WorldPosition = mul(float4(mv.Position, 1.f), WorldMatrix);
	
	// ����������תΪ�ӿڲü��ռ�����
	Out.PositionH = mul(Out.WorldPosition, ViewportProjectionMatrix);
	
	return Out;

}


float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];

	return MatConstBuffer.BaseColor;
}


