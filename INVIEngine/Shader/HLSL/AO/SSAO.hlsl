#include "AOCommon.hlsl"
#include "AOFunction.hlsl"

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
	
	float3 NoiseSampleValue = SampleNoiseMap.SampleLevel(TextureSampler, mvOut.Texcoord, 0).xyz;
	
	float3 DepthNdc = SampleDepthMap.SampleLevel(DepthSampler, mvOut.Texcoord, 0).rrr;
	
	float A_ViewDepth = DepthNDCToView(DepthNdc.r);
	
	float3 AViewPos = A_ViewDepth * mvOut.ViewPosition.xyz / mvOut.ViewPosition.z;
	
	// ���벨�в��������⣬��û�����ķ�������õ���ֵ��[0,1]
	float3 AmbientLightDirection = SampleNoiseMap.SampleLevel(TextureSampler, mvOut.Texcoord, 0.f);
	
	// �� [0,1] ӳ�䵽 [-1, 1]
	AmbientLightDirection = AmbientLightDirection * 2.f - 1.f;
	
	for (int i = 0; i < SAMPLE_VOLUME_NUM; i++)
	{
		float3 SampleVolume = SampleVolumeData[i].xyz;
		
		float3 SampleVolumeViewPos = mul(SampleVolume, InversiveProjectionMatrix);
		
		SampleVolumeViewPos = SampleVolumeViewPos / SampleVolumeViewPos.w;
		
		float3 SampleVolumeNdc = SampleVolumeViewPos.xy / SampleVolumeViewPos.z;
		
		float SampleDepth = DepthNDCToView(SampleVolumeNdc.z);
		
		float3 SampleViewPos = SampleDepth * SampleVolumeViewPos / SampleVolumeViewPos.z;
		
		float3 SampleNormal = normalize(SampleNormalMap.SampleLevel(TextureSampler, SampleVolumeNdc.xy, 0).xyz);
		
		float3 SampleDirection = SampleViewPos - AViewPos;
		
		float SampleDistance = length(SampleDirection);
		
		SampleDirection = normalize(SampleDirection);
		
		float NdotL = dot(SampleNormal, SampleDirection);
		
		float Occlusion = max(0.f, NdotL);
		
		float OcclusionFade = saturate((SampleDistance - OcclusionFadeStart) / (OcclusionFadeEnd - OcclusionFadeStart));
		
		Occlusion = Occlusion * lerp(1.f, 1.f - OcclusionFade, Occlusion);
		
		Occlusion = saturate((SampleDistance - OcclusionRadius) / OcclusionRadius);
		
		// ͨ��������ķ���Ͳ�����ķ����������ڱ�ֵ
		float3 AmbientOcclusion = AmbientLightDirection * SampleNormal;
		
		// ͨ���ڱ�ֵ�����㻷����
		NoiseSampleValue += AmbientOcclusion * Occlusion;
	}
	
	// return float4(NormalizedSampleValue, 1.f);
	// return float4(DepthNdc.rrr, 1.f);
		return float4(NoiseSampleValue, 1.f);
}

