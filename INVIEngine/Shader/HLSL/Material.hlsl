#include "ShaderCommon.hlsl"
#include "ShaderFunctionLibrary.hlsl"

// �������
struct FMaterial
{
    // ͨ�ò���
    float4 BaseColor;
};


// �����
float3 FresnelSchlick(float3 F0, float3 PointNormal, float3 Direction, int M)
{
    return F0 + (1.f-F0) * pow(1.f - saturate(dot(PointNormal, Direction)), M);
}

// ��ȡbasecolor
float4 GetMaterialBaseColor(MaterialConstBuffer MatConstbuffer, float2 Texcoord)
{
	if (MatConstbuffer.BaseColorIndex == -1)
	{
		return MatConstbuffer.BaseColor;
	}

	// ������� (v���������ʽ������UV��
	return SimpleTexture2DMap[MatConstbuffer.BaseColorIndex].Sample(TextureSampler, Texcoord);
}

// ��ȡ������ͼ
float3 GetMaterialNormal(MaterialConstBuffer MatConstbuffer, float2 Texcoord, float3 unitWorldNormal, float3 worldTangent)
{
	if (MatConstbuffer.NormalIndex != -1)
	{
		float4 simpleNormal = SimpleTexture2DMap[MatConstbuffer.NormalIndex].Sample(AnisotropicSampler, Texcoord);

		// �� ��0��1����simpleNormalֵ ӳ�䵽 ��-1�� 1��
		float3 normalTangentSpace = 2.f * simpleNormal.rgb - 1.f;

		// �õ�����TBN
		float3x3 tbn = GetTBNMatrix(unitWorldNormal, worldTangent);

		// �����߿ռ��µĲ�������תΪ���編��
		return mul(normalTangentSpace, tbn);

	}

	return unitWorldNormal;
}

// ��ȡ�߹���ͼ
float4 GetMaterialSpecular(MaterialConstBuffer MatConstbuffer, float2 Texcoord)
{
	if (MatConstbuffer.SpecularIndex == -1)
	{
		return float4(MatConstbuffer.SpecularColor, 1.0f);
	}

	// ������� (v���������ʽ������UV��
	return SimpleTexture2DMap[MatConstbuffer.SpecularIndex].Sample(TextureSampler, Texcoord);
}
