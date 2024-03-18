// ����״̬
SamplerState TextureSampler : register(s0); // ��ͼ������
SamplerState AnisotropicSampler : register(s1); // �������Բ�����
SamplerComparisonState ShadowSampler : register(s2); // ��Ӱ������
SamplerState PointClampSampler : register(s3); // �����Ʋ�����

Texture2D SampleNormalMap : register(t0); // ����

static const float2 TextureCoordinates[6] =
{
	float2(0.f, 1.f),
	float2(0.f, 0.f),
	float2(1.f, 0.f),
	
	float2(0.f, 1.f),
	float2(1.f, 0.f),
	float2(1.f, 1.f),
};

struct MeshVertexOut
{
	float4 ViewPosition : POSITION;
	float4 Position : SV_POSITION;
	float2 Texcoord : TEXCOORD; // UV
};

cbuffer CBufferSSAOView : register(b0)
{
	float4x4 InversiveProjectionMatrix;	// ������ͶӰ����
	float4x4 ProjectionMatrix;			// �������ͶӰ����
	float4x4 TextureProjectionMatrix;	// ����ռ��ͶӰ����
};


// `SV_VertexID` ������ɫ��������ʹ�õ�ϵͳ���ñ����������ڱ�ʾ��ǰ�����ڶ��㻺�����е��������������ͨ�����ڼ���ÿ�������Ψһ��ʶ����ִ���붥����صĲ�����

//��ͼ����Ⱦ�����У� ����ͼԪ��
//�������Σ� �ᱻ�ֽ�Ϊ������㣬
//���洢�ڶ��㻺�����С� ����Ⱦ����ִ�ж�����ɫ��ʱ��
//����Ϊÿ���������һ�ζ�����ɫ�������� ��Ϊ�ö����ṩһЩ���ñ�����
//����` SV_VertexID`
//��������֮һ��

//������һЩʹ��`
//SV_VertexID` �ĳ�����;��

//1.
//����Ψһ��ʶ���� ͨ��ʹ��`
//SV_VertexID` ��������ÿ�������Ψһ��ʶ����
//�����ִ�ж��㶯���� ����λ�ƻ������붥����صļ�������á�

//2.
//�����������ݣ� ���Ӷ��㻺�����ж�ȡ��������ʱ��
//����ʹ��` SV_VertexID`
//��Ϊ�����������ض���������ݡ� ���ڶ���任��
//���߼���Ȳ����о����õ���

//3. ������ɫ�����̿��ƣ�
//����` SV_VertexID`
//��ֵ�� �����ڶ�����ɫ����ִ�в�ͬ�Ĵ���·����
//���磬 ���Ը���`
//SV_VertexID` ִ���ض��ı任��
//��������Ȳ����� ��ʵ��һЩ�����ض���Ч����

//��Ҫע����ǣ�` SV_VertexID`
//��ֵ�Ǵ�0��ʼ�����ģ� ��Ӧ�ڶ����ڶ��㻺�����е�������
//���ڶ�����ɫ������ֻ���ģ� �޷��ֶ��޸ġ�

//�ܶ���֮��` SV_VertexID`
//��һ��ϵͳ���ñ����� ���ڱ�ʾ�����ڶ��㻺�����е�������
//�������������ɶ����Ψһ��ʶ���� �������ݵ������Ͷ�����ɫ���е����̿��ơ�
MeshVertexOut VSMain(uint VertexID : SV_VertexID)
{
	
	MeshVertexOut outV;
	
	outV.Texcoord = TextureCoordinates[ VertexID];
	
	// ������Ҫ�� 0-1 ӳ�䵽NDC�ռ䣬����������Ҫ��UV�������2��Ȼ���ȥ1��Ȼ����ΪY���Ƿ��ģ�����������Ҫ��Y���UV�������-1
	outV.Position = float4(outV.Texcoord.x * 2.f - 1.f, (outV.Texcoord.y * 2.f - 1.f) * -1.f, 0.f, 1.f);
	
	// ����ת�����ӿڿռ���
	float4 PositionH = mul(outV.Position, InversiveProjectionMatrix);
	
	// ת����������
	outV.ViewPosition.xyz = PositionH.xyz / PositionH.w;

	return outV;
}

float4 PSMain(MeshVertexOut mvOut) : SV_TARGET
{
	float3 NormalizedSampleValue = normalize(SampleNormalMap.SampleLevel(TextureSampler, mvOut.Texcoord, 0).xyz);
	return float4(NormalizedSampleValue, 1.f);
}

