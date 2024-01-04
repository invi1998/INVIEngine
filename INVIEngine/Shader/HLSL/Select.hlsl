// ����ѡ�������shader

#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // λ��
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 PositionH : SV_POSITION;
	float2 Texcoord : TEXCOORD;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];
	
	MeshVertexOut Out = (MeshVertexOut) 0;
	
	// ��ģ�͵Ķ�������תΪ��������
	float4 PositionWorld = mul(float4(mv.Position, 1.f), WorldMatrix);
	
	// ��ȡָ�������������
	float3 ViewDir = normalize(CameraPosition.xyz - PositionWorld.xyz);
	
	// ��ģ�͵Ķ����������������ָ��ķ����ƶ�һ�㣬�����Ϳ���������ѡ�е������������ǰ��Ӷ�����Ⱦ����
	PositionWorld.xyz += ViewDir * 0.01f;
	
	// ����������תΪ�ӿڲü��ռ�����
	Out.PositionH = mul(PositionWorld, ViewportProjectionMatrix);
	
	// UV
    // �Ƚ������uv�����ģ�͵�����任��ˣ��õ�����任���UV
	float4 TextureTexcoord = mul(float4(mv.Texcoord, 0.f, 1.f), TextureTransformationMatrix);
    // Ȼ��������ʵı任������ˣ��õ����ʱ任���UV�������������յĶ���UV��
	Out.Texcoord = mul(TextureTexcoord, MatConstBuffer.MaterialProjectionMatrix).xy;
	
	return Out;

}


float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	return float4(0.9f, 0.23f, 0.11f, 1.0f);
}


