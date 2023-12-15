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
	// 设置采样方式为默认
	SamplerDescriptors.push_back(
		CD3DX12_STATIC_SAMPLER_DESC(
			0,			// 指定寄存器编号
			D3D12_FILTER_MIN_MAG_MIP_POINT	// 纹理采样方式
		));

	// 设置法线贴图的采样方式
	SamplerDescriptors.push_back(
		CD3DX12_STATIC_SAMPLER_DESC(
			1,
			D3D12_FILTER_ANISOTROPIC,		// 各向异性
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			0,		// Mipmap级别偏移值。如果我们将该值设置为0，那么我们的mipmap采样层级是不会变的，固定1层。如果设置为2，mipmap层级就为3.
			8		// 各向异性过滤的最大采样等级。
		)
	);

	// 阴影贴图采样方式
	SamplerDescriptors.push_back(
		CD3DX12_STATIC_SAMPLER_DESC(
			2,
			D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,		// 过滤类型：比较过滤
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,		// 边界采样方式 U	
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,		// 边界采样方式 V
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,		// 边界采样方式 W
			0,		// Mipmap级别偏移值。如果我们将该值设置为0，那么我们的mipmap采样层级是不会变的，固定1层。如果设置为2，mipmap层级就为3.
			16,		// 各向异性过滤的最大采样等级。
			D3D12_COMPARISON_FUNC_LESS_EQUAL,		// 比较函数：小于等于
			D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK		// 边界颜色 黑色
		)
	);
}
