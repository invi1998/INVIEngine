#include "EngineMinimal.h"
#include "NormalBuffer.h"

FNormalBuffer::FNormalBuffer()
{
	// 初始化Buff数据存储格式
	Format = DXGI_FORMAT_R16G16B16A16_FLOAT;	// rgba16 存储法线
}

FNormalBuffer::~FNormalBuffer()
{
}

void FNormalBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	FRenderBuffer::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FNormalBuffer::SetBufferSize(int wid, int hei)
{
	FRenderBuffer::SetBufferSize(wid, hei);
}

void FNormalBuffer::PreDraw(float DeltaTime)
{
	FRenderBuffer::PreDraw(DeltaTime);
}

void FNormalBuffer::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FRenderBuffer::UpdateCalculations(delta_time, viewport_info);
}

void FNormalBuffer::Build(const XMFLOAT3& center)
{
	FRenderBuffer::Build(center);
}

void FNormalBuffer::Draw(float deltaTime)
{
	FRenderBuffer::Draw(deltaTime);
}

void FNormalBuffer::ResetView(int wid, int hei)
{
	FRenderBuffer::ResetView(wid, hei);
}

void FNormalBuffer::BuildDescriptor()
{
}

void FNormalBuffer::BuildRenderTargetRTV()
{
}

void FNormalBuffer::BuildSRVDescriptor()
{
}

void FNormalBuffer::BuildRTVDescriptor()
{
}

void FNormalBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
	// 资源描述
	CD3DX12_RESOURCE_DESC CubeMapResourceDesc{};

	memset(&CubeMapResourceDesc, 0, sizeof(CD3DX12_HEAP_PROPERTIES));

	CubeMapResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;		// 资源维度2D纹理
	CubeMapResourceDesc.Alignment = 0;		// 对齐方式
	CubeMapResourceDesc.Width = Width;		// 资源宽度
	CubeMapResourceDesc.Height = Height;	// 资源高度
	CubeMapResourceDesc.DepthOrArraySize = 1;		// 资源数量
	CubeMapResourceDesc.MipLevels = 1;		// mipmap
	CubeMapResourceDesc.Format = Format;		// 数据格式
	CubeMapResourceDesc.SampleDesc.Count = 1;	// 采样描述-数量-1
	CubeMapResourceDesc.SampleDesc.Quality = 0;	// 采样描述-质量-0
	CubeMapResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// 纹理布局方式-未知布局（由驱动自行决定）
	CubeMapResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // 资源标记-标记资源可以被允许设置为我们的渲染目标（renderTarget）


	// 创建默认堆
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE ClearValue = CD3DX12_CLEAR_VALUE(Format, Colors::Blue);	// 清除值，我们给法线缓冲区填充蓝色，因为法线的默认值是(0,0,1)

	// 创建提交资源
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&CubeMapResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&ClearValue,
		IID_PPV_ARGS(OutResource.GetAddressOf())
	));
}
