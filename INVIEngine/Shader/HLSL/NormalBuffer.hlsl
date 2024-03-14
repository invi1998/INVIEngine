#include "Material.hlsl"
#include "PBR.hlsl"
#include "SkyFunction.hlsl"
#include "ShadowFunction.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // λ��
	float3 Normal : NORMAL; // ����
	float3 UTangent : TANGENT; // ���ߣ�U����
	float2 Texcoord : TEXCOORD; // UV
};

struct MeshVertexOut
{
	float4 WorldPosition : POSITION;
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float3 UTangent : TANGENT; // ���ߣ�U����
	float2 Texcoord : TEXCOORD; // UV
};


MeshVertexOut VSMain(MeshVertexIn mv)
{
	MaterialConstBuffer MatConstbuffer = Materials[MaterialID];
	
	MeshVertexOut outV;

	// ��ȡ�������������
	outV.WorldPosition = mul(float4(mv.Position, 1.0f), WorldMatrix);
    
    // ��ģ��ת����βü��ռ�
	outV.Position = mul(outV.WorldPosition, ViewportProjectionMatrix);
	
	// ���ߣ���ģ�Ϳռ�ת������ռ䣩
	outV.Normal = mul(mv.Normal, (float3x3) NormalMatrix);

    // ���� (�����߿ռ�ת������ռ䣩
	outV.UTangent = mul(mv.UTangent, (float3x3) NormalMatrix);
    
    // UV
    // �Ƚ������uv�����ģ�͵�����任��ˣ��õ�����任���UV
	float4 TextureTexcoord = mul(float4(mv.Texcoord, 0.f, 1.f), TextureTransformationMatrix);
    // Ȼ��������ʵı任������ˣ��õ����ʱ任���UV�������������յĶ���UV��
	outV.Texcoord = mul(TextureTexcoord, MatConstbuffer.MaterialProjectionMatrix).xy;

	return outV;
}

float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	float3 NormallizedWorldNormal = normalize(mvOut.Normal);
	
	// ������ת���ӿڿռ�
	float3 ViewNormal = mul(NormallizedWorldNormal, (float3x3)ViewportProjectionMatrix);
	
	return float4(ViewNormal, 0.0f);
}
