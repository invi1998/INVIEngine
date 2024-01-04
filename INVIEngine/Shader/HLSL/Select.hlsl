// ����ѡ�������shader

#include "Material.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // λ��
	float4 Color : COLOR; // ��ɫ
	float3 Normal : NORMAL; // ����
	float3 UTangent : TANGENT; // ���ߣ�U����
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 PositionH : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Texcoord : TEXCOORD;
	float4 WorldPosition : POSITION;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];
	
	MeshVertexOut Out = (MeshVertexOut) 0;
	
	// ��ģ�͵Ķ�������תΪ��������
	Out.WorldPosition = mul(float4(mv.Position, 1.f), WorldMatrix);
	
	// ��ȡָ�������������
	float3 ViewDir = normalize(CameraPosition.xyz - Out.WorldPosition.xyz);
	
	// ��ģ�͵Ķ����������������ָ��ķ����ƶ�һ�㣬�����Ϳ���������ѡ�е������������ǰ��Ӷ�����Ⱦ����
	Out.WorldPosition.xyz += ViewDir * 0.01f;
	
	// ����������תΪ�ӿڲü��ռ�����
	Out.PositionH = mul(Out.WorldPosition, ViewportProjectionMatrix);
	
	// UV
    // �Ƚ������uv�����ģ�͵�����任��ˣ��õ�����任���UV
	float4 TextureTexcoord = mul(float4(mv.Texcoord, 0.f, 1.f), TextureTransformationMatrix);
    // Ȼ��������ʵı任������ˣ��õ����ʱ任���UV�������������յĶ���UV��
	Out.Texcoord = mul(TextureTexcoord, MatConstBuffer.MaterialProjectionMatrix).xy;
	
	// ����
	Out.Normal = mul(mv.Normal, (float3x3) WorldMatrix);
	
	return Out;

}


float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	float3 ModelNormal = normalize(mvOut.Normal);
	float3 ViewDirection = normalize(CameraPosition.xyz - mvOut.WorldPosition.xyz);
	float DotDiffValue = pow(1.f - max(dot(ModelNormal, ViewDirection), 0.0), 4.f);
	
	return float4(0.98f, 0.12f, 0.22f, 1.0f) * DotDiffValue;
}


