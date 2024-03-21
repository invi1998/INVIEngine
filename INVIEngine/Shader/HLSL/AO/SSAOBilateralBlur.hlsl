#include "AOFunc.hlsl"

struct MeshVertexOutBlur
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

MeshVertexOutBlur VSMain(uint VertexID : SV_VertexID)
{
	MeshVertexOutBlur Out = (MeshVertexOutBlur) 0.f;

	Out.TexCoord = TextureCoordinates[VertexID];

	//0-1  NDC�ռ� 
	Out.Position = float4(2.f * Out.TexCoord.x - 1.f, 1.f - 2.f * Out.TexCoord.y, 0.f, 1.f);

	return Out;
}

float4 PSMain(MeshVertexOutBlur mvOut) : SV_TARGET
{
	float BlurWeight[12] =
	{
		Float4BlurWeights[0].x, Float4BlurWeights[0].y, Float4BlurWeights[0].z, Float4BlurWeights[0].w,
		Float4BlurWeights[1].x, Float4BlurWeights[1].y, Float4BlurWeights[1].z, Float4BlurWeights[1].w,
		Float4BlurWeights[2].x, Float4BlurWeights[2].y, Float4BlurWeights[2].z, Float4BlurWeights[2].w,
	};
	
	float2 TexOffset;
	if (bHorizontalBlur)
	{
		TexOffset = float2(1.f / InversionTextureSize.x, 0.f);	// ������x���ϵ�ƫ��
	}
	else
	{
		TexOffset = float2(0.f, 1.f / InversionTextureSize.y);	// ������y���ϵ�ƫ��
	}

	// ����Ȩ�ض�Ӧ����ɫ�����ߣ����
	float weightSum = BlurWeight[BlurRadius];
	float4 Color = weightSum * SampleAcceptMap.SampleLevel(TextureSampler, mvOut.TexCoord, 0);		// ��ȡ��������ͼ����ɫ

	float3 Normal = SampleNormalMap.SampleLevel(TextureSampler, mvOut.TexCoord, 0).rgb;		// ��������

	float NDCDepth = SampleDepthMap.SampleLevel(DepthSampler, mvOut.TexCoord, 0).r;		// �������
	float Depth = DepthNDCToView(NDCDepth);		// NDC���ת��Ϊ��ͼ�ռ����

	for (float i = -BlurRadius; i <= BlurRadius; ++i)
	{
		if (i == 0)
		{
			continue;	// ������������
		}

		float2 uv = mvOut.TexCoord + i * TexOffset;

		float3 blurNormal = SampleNormalMap.SampleLevel(TextureSampler, uv, 0).rgb;		// ��������
		float blurNDCDepth = SampleDepthMap.SampleLevel(DepthSampler, uv, 0).r;		// �������
		float blurDepth = DepthNDCToView(blurNDCDepth);

		if (abs(Depth - blurDepth) > 0.1f)
		{
			continue;	// ��Ȳ����������
		}
		if (dot(Normal, blurNormal) < 0.8f)
		{
			continue;	// ���߲����������
		}

		float weight = BlurWeight[BlurRadius + i];
		Color += weight * SampleAcceptMap.SampleLevel(TextureSampler, uv, 0);

		weightSum += weight;

	}

	// float3 tempColor = SampleAcceptMap.SampleLevel(TextureSampler, mvOut.TexCoord, 0).rgb;
	return Color / weightSum;
}
