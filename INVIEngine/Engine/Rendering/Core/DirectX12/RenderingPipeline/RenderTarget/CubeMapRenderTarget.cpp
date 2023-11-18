#include "EngineMinimal.h"
#include "CubeMapRenderTarget.h"

FCubeMapRenderTarget::FCubeMapRenderTarget(): Width(256), Height(256), Format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
	CPURenderTargetView.resize(6);

	ResetViewport();

	ResetScissorRect();
}

void FCubeMapRenderTarget::Init(UINT w, UINT h, const DXGI_FORMAT& format)
{
	Width = w;
	Height = h;
	Format = format;

	ResetViewport();

	ResetScissorRect();

	BuildRenderTarget();

	BuildSRVDescriptor();

	BuildRTVDescriptor();

	
}

void FCubeMapRenderTarget::ResetViewport()
{
	Viewport = {
		0.f,
		0.f,
		static_cast<float>(Width),
		static_cast<float>(Height),
		0.f,	// 最小深度
		1.f		// 最大深度
	};
}

void FCubeMapRenderTarget::ResetScissorRect()
{
	ScissorRect = {
		0,
		0,
		static_cast<LONG>(Width),
		static_cast<LONG>(Height)
	};
}

void FCubeMapRenderTarget::BuildRenderTarget()
{
	// 资源描述
	CD3DX12_RESOURCE_DESC CubeMapResourceDesc{};

	memset(&CubeMapResourceDesc, 0, sizeof(CD3DX12_HEAP_PROPERTIES));

	CubeMapResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;		// 资源维度2D纹理
	CubeMapResourceDesc.Alignment = 0;		// 对齐方式
	CubeMapResourceDesc.Width = Width;		// 资源宽度
	CubeMapResourceDesc.Height = Height;	// 资源高度
	CubeMapResourceDesc.DepthOrArraySize = 6;		// 资源数量（6个，因为我们的是立方体贴图）（如果你的资源是3D维度，那么这个值就是指的资源深度，如果是1D, 2D，那么这个值就是指资源数组的长度）
	CubeMapResourceDesc.MipLevels = 1;		// mipmap
	CubeMapResourceDesc.Format = Format;		// 数据格式
	CubeMapResourceDesc.SampleDesc.Count = 1;	// 采样描述-数量-1
	CubeMapResourceDesc.SampleDesc.Quality = 0;	// 采样描述-质量-0
	CubeMapResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// 纹理布局方式-未知布局（由驱动自行决定）
	CubeMapResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // 资源标记-标记资源可以被允许设置为我们的渲染目标（renderTarget）


	// 创建默认堆
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// 创建提交资源
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&CubeMapResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(RenderTargetMap.GetAddressOf())
	));
}

void FCubeMapRenderTarget::BuildSRVDescriptor()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRV_Desc{};

	SRV_Desc.Format = Format;		// 设置纹理数据格式
	SRV_Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;	// 设置SRV的维度 CUBE_MAP
	SRV_Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;		// 着色器组件映射的结构体。它可以用于指定不同颜色通道之间的映射关系，
	// D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING是DXGI_ENUM中的一个常量，它代表了默认的着色器组件映射方式。具体地，它表示将RGBA四个颜色通道分别映射到输入数据的RGBA四个颜色通道 (0, 1, 2, 3)

	// 设置cube map
	SRV_Desc.TextureCube.MipLevels = 1;
	SRV_Desc.TextureCube.MostDetailedMip = 0;
	SRV_Desc.TextureCube.ResourceMinLODClamp = 0.f;


	GetD3dDevice()->CreateShaderResourceView(
		GetRenderTarget(),
		&SRV_Desc,
		CPUShaderResourceView
	);
}

void FCubeMapRenderTarget::BuildRTVDescriptor()
{
	for (size_t i = 0; i < 6; i++)
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTV_Desc{};

		RTV_Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;	// 指定资源视图维度
		RTV_Desc.Format = Format;	// 数据格式

		// 设置Texture2DArray
		RTV_Desc.Texture2DArray.MipSlice = 0;	// MIP_MAP切片(0,只有一个mip map）
		RTV_Desc.Texture2DArray.PlaneSlice = 0;
		// 索引mip——map资源的公式
		// MipSlice + ArraySize * MipLevels
		RTV_Desc.Texture2DArray.FirstArraySlice = i;
		RTV_Desc.Texture2DArray.ArraySize = 1;		// 每一个RTV里只有一个贴图资源
		

		GetD3dDevice()->CreateRenderTargetView(
			GetRenderTarget(),
			&RTV_Desc,
			CPURenderTargetView[i]
		);
	}
}

