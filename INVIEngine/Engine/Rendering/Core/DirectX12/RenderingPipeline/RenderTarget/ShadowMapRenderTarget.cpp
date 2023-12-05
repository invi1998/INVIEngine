#include "EngineMinimal.h"
#include "ShadowMapRenderTarget.h"

FShadowMapRenderTarget::FShadowMapRenderTarget() : FRenderTarget()
{
}

void FShadowMapRenderTarget::Init(UINT w, UINT h, const DXGI_FORMAT& format)
{
	FRenderTarget::Init(w, h, format);
}

void FShadowMapRenderTarget::BuildRenderTarget()
{
	// 资源描述
	CD3DX12_RESOURCE_DESC CubeMapResourceDesc{};

	memset(&CubeMapResourceDesc, 0, sizeof(CD3DX12_HEAP_PROPERTIES));

	CubeMapResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;		// 资源维度2D纹理
	CubeMapResourceDesc.Alignment = 0;		// 对齐方式
	CubeMapResourceDesc.Width = Width;		// 资源宽度
	CubeMapResourceDesc.Height = Height;	// 资源高度
	CubeMapResourceDesc.DepthOrArraySize = 1;		// 资源数量（1个，深度阴影贴图）
	CubeMapResourceDesc.MipLevels = 1;		// mipmap
	CubeMapResourceDesc.Format = Format;		// 数据格式
	CubeMapResourceDesc.SampleDesc.Count = 1;	// 采样描述-数量-1
	CubeMapResourceDesc.SampleDesc.Quality = 0;	// 采样描述-质量-0
	CubeMapResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// 纹理布局方式-未知布局（由驱动自行决定）
	CubeMapResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // 资源标记-标记资源可以被允许设置为我们的深度模板测试


	// 创建默认堆
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	// 创建提交资源
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&CubeMapResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&clearValue,
		IID_PPV_ARGS(RenderTargetMap.GetAddressOf())
	));
}

void FShadowMapRenderTarget::BuildSRVDescriptor()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRV_Desc{};

	SRV_Desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;		// 设置纹理数据格式
	SRV_Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;		// 设置SRV的维度 Texture2D
	SRV_Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;		// 着色器组件映射的结构体。它可以用于指定不同颜色通道之间的映射关系，
	// D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING是DXGI_ENUM中的一个常量，它代表了默认的着色器组件映射方式。具体地，它表示将RGBA四个颜色通道分别映射到输入数据的RGBA四个颜色通道 (0, 1, 2, 3)

	// 设置Texture2D
	SRV_Desc.Texture2D.MipLevels = 1;		// mipmap
	SRV_Desc.Texture2D.MostDetailedMip = 0;	// 最详细的mipmap
	SRV_Desc.Texture2D.PlaneSlice = 0;		// 平面切片


	GetD3dDevice()->CreateShaderResourceView(
		GetRenderTarget(),
		&SRV_Desc,
		CPUShaderResourceView
	);
}

void FShadowMapRenderTarget::BuildRTVDescriptor()
{
}
