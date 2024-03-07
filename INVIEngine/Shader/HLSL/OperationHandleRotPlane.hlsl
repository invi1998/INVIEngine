// ���������shader

#include "Material.hlsl"
#include "ShadowFunction.hlsl"

struct MeshVertexIn
{
	float3 Position : POSITION; // λ��
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
	
	// ��ģ�͵Ķ�������תΪ��������
	Out.WorldPosition = mul(float4(mv.Position, 1.f), WorldMatrix);
	
	// ����������תΪ�ӿڲü��ռ�����
	Out.PositionH = mul(Out.WorldPosition, ViewportProjectionMatrix);
	
	// UV����
	float4 TextureTexcoord = mul(float4(mv.TexCoord, 0.f, 1.f), TextureTransformationMatrix);
	Out.TexCoord = mul(TextureTexcoord, MatConstBuffer.MaterialProjectionMatrix).xy;
	
	return Out;

}


float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	MaterialConstBuffer MatConstBuffer = Materials[MaterialID];
	
	float r = SampleBuildCDTexture(mvOut.TexCoord);
	
	r += MatConstBuffer.Param0; // CD����
	r = floor(r);	// ����ȡ��
	r *= 0.5f;	// ͸����
	r *= 1.f;	// ����
	
	// ��ȡ������ɫ
	float4 color = GetMaterialBaseColor(MatConstBuffer, mvOut.TexCoord);
	
	// ����Alpha���
	float alpha = abs(color.a * r + color.r);	// aͨ������r����rͨ��
	
	// �ü��߽�
	if (alpha <= 0.1f)
	{
		alpha = 0.f;
	}

	return float4(0.76f, 0.14f, 1.f, alpha);
	
	
	
	// return float4(MatConstBuffer.Param0, MatConstBuffer.Param0, MatConstBuffer.Param0, 1.f); // DEBUG

}


