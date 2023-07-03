
// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
cbuffer ConstBuffer : register(b0)
{
	float4x4 World;
}

struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};

struct MeshVertexOut
{
	float3 Position : VS_POSITION;
	float4 Color : COLOR;
};

MeshVertexOut VS_Main(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// ��ģ��ת����βü��ռ�
	outV.Position = mul(float4(mv.Position, 1.0f), World);
	outV.Color = mv.Color;

	return outV;
}

float4 PS_Main(MeshVertexOut vsOut) : SV_TARGET
{
	return vsOut.Color;
}
