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
}

void FShadowMapRenderTarget::BuildRTVDescriptor()
{
}
