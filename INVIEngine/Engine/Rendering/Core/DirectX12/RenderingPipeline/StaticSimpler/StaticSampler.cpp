#include "EngineMinimal.h"
#include "StaticSampler.h"

FStaticSampler::FStaticSampler()
{
}

CD3DX12_STATIC_SAMPLER_DESC* FStaticSampler::GetData()
{
	return SamplerDescriptors.data();
}

int FStaticSampler::GetSize() const
{
	return SamplerDescriptors.size();
}

void FStaticSampler::BuildStaticSampler()
{
	// ���ò�����ʽΪĬ��
	SamplerDescriptors.push_back(
		CD3DX12_STATIC_SAMPLER_DESC(
			0,			// ָ���Ĵ������
			D3D12_FILTER_MIN_MAG_MIP_POINT	// ���������ʽ
		));

	// ���÷�����ͼ�Ĳ�����ʽ
	SamplerDescriptors.push_back(
		CD3DX12_STATIC_SAMPLER_DESC(
			1,
			D3D12_FILTER_ANISOTROPIC,		// ��������
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			0,		// Mipmap����ƫ��ֵ��������ǽ���ֵ����Ϊ0����ô���ǵ�mipmap�����㼶�ǲ����ģ��̶�1�㡣�������Ϊ2��mipmap�㼶��Ϊ3.
			8		// �������Թ��˵��������ȼ���
		)
	);
}
