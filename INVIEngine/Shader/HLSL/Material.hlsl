#ifndef __SHADER_MATERIAL__
#define __SHADER_MATERIAL__


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

// ��ȡ���䣨�ӽ������Ͷ������編�ߵķ���������
float3 GetReflect(float3 UnitWorldNormal)
{
	return reflect(-CameraPosition, UnitWorldNormal);
}

// ��ȡ���������ɫ
float3 GetReflectionSampleColor(float3 UnitWorldNormal, float3 NewReflect)
{
	return SimpleCubeMap[0].Sample(TextureSampler, NewReflect).rgb;
}

// ��ȡ�����(������ǰ������ɫ��ǿ�ȣ�
float GetShininess(MaterialConstBuffer MaterialBuff)
{
	// 1 - ���ʴֲڶ�
	return 1.f - MaterialBuff.MaterialRoughness;
}

// ��ȡ���������
float3 FresnelSchlickFactor(MaterialConstBuffer MaterialBuff, float3 UnitWorldNormal, float3 inReflect)
{
	return FresnelSchlick(MaterialBuff.FresnelF0, UnitWorldNormal, inReflect, 5);
}

// ���ջ�ȡ�ķ�����ɫ
float3 GetReflectionColor(MaterialConstBuffer MaterialBuff, float3 UnitWorldNormal)
{
	float3 NewReflect = GetReflect(UnitWorldNormal);

	// ���������ɫ
	float3 ReflectSampleColor = GetReflectionSampleColor(UnitWorldNorma, NewReflect);

	// �ֲڶ�
	float3 shiness = GetShininess(MaterialBuff);

	// �����
	float3 Fresnel = FresnelSchlickFactor(MaterialBuff, UnitWorldNormal, NewReflect);

	return saturate(ReflectSampleColor * shiness * Fresnel);

}

#endif
