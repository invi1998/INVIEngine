
cbuffer MeshConstBuffer : register(b0)
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

cbuffer MaterialConstBuffer : register(b2)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	float4x4 MaterialProjectionMatrix;
}

cbuffer LightConstBuffer : register(b2)
{
	// ��������������(������Ҫ��������ĳ��������������ݼĴ浽�Ĵ�����Ĵ�����15��b0-b14��Ȼ��ӼĴ������ȡ����ʹ��)
	float3 LightIntensity;		// ����ǿ��
	float3 LightDirection;		// ���շ���
}

struct MeshVertexIn
{
	float3 Position : POSITION;		// λ��
	float4 Color : COLOR;			// ��ɫ
	float3 Normal : NORMAL;			// ����
};

struct MeshVertexOut
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

MeshVertexOut VSMain(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// ��ģ��ת����βü��ռ�
	float4 Position = mul(float4(mv.Position, 1.0f), WorldMatrix);
	outV.Position = mul(Position, ViewportProjectionMatrix);

    outV.Normal = normalize(mul(mv.Normal, (float3x3) WorldMatrix));

	// ���շ����ȡ��
    float diff = max(0.f, dot(outV.Normal, normalize(-LightDirection)));

	outV.Color = mv.Color * diff;

	return outV;
}

float4 PSMain(MeshVertexOut vsOut) : SV_TARGET
{
    // float4 color;
    return vsOut.Color;
}
