
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

cbuffer LightConstBuffer : register(b3)
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

float2 tri(in float2 x)
{
    float2 h = frac(x * 0.5f) - 0.5f;
    return 1.f - 2.f * abs(h);
}

float checkersGrid(float2 uv, float2 ddx, float2 ddy)
{
    float2 w = max(abs(ddx), abs(ddy)) + 0.01f;
    float2 i = (tri(uv + 0.5 * w) - tri(uv - 0.5 * w)) / w;
    return 0.5 - 0.5 * i.x * i.y;
}


MeshVertexOut VSMain(MeshVertexIn mv)
{
	MeshVertexOut outV;

	// ��ģ��ת����βü��ռ�
    float4 Position = mul(float4(mv.Position, 1.0f), WorldMatrix);
    outV.Position = mul(Position, ViewportProjectionMatrix);

    outV.Normal = mul(mv.Normal, (float3x3) WorldMatrix);

	// ���շ����ȡ��
    float diff = max(dot(normalize(outV.Normal), normalize(-LightDirection)), 0.0f);

	// ������
    float4 ambient = float4(0.19f, 0.18f, 0.17f, 1.0f);

    outV.Color = mv.Color * diff + ambient;

	// ٤��У��
    outV.Color = sqrt(outV.Color);

	return outV;
}

//float3 render(float2 uv, in float2 px, in float2 py)
//{
//	// ƽ��
//    float3 rdx = normalize(ViewportProjectionMatrix * float3(px, fl));
//    float3 rdy = normalize(ViewportProjectionMatrix * float3(py, fl));

//    float3 ddx = ro.y * (rd / rd.y - rdx / rdx.y);
//    float3 ddy = ro.y * (rd / rd.y - rdy / rdy.y);

//    c = float3(0.) + float3(0.9) * checkersGrid(p.xz, ddx.xz, ddy.xz);
//}

float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
    float4 ambientLight = { 0.15f, 0.15f, 0.22f, 1.0f };

    float ModelNormal = normalize(mvOut.Normal);
    float NormalizeLightDirection = normalize(-LightDirection);
    float DotDiffValue = max(dot(ModelNormal, NormalizeLightDirection), 0.0f);

    mvOut.Color = mvOut.Color * DotDiffValue + ambientLight;
	// ٤��У��
    mvOut.Color = sqrt(mvOut.Color);
    return mvOut.Color;

}
