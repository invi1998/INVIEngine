
cbuffer ObjectConstBuffer : register(b0)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	float4x4 WorldMatrix;
}


cbuffer ViewportConstBuffer : register(b1)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	// �ӿ�ͶӰ����
	float4x4 ViewportProjectionMatrix;
}

struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};

struct MeshVertexOut
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// ��ģ��ת����βü��ռ�
	float4 Position = mul(float4(mv.Position, 1.0f), WorldMatrix);
	outV.Position = mul(Position, ViewportProjectionMatrix);

	outV.Color = mv.Color;

	return outV;
}

float4 PSMain(MeshVertexOut vsOut) : SV_TARGET
{
	return vsOut.Color;
}
