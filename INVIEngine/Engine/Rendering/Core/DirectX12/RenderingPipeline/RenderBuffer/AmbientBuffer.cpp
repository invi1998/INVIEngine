#include "EngineMinimal.h"
#include "AmbientBuffer.h"

#include "Config/EngineRenderConfig.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

FAmbientBuffer::FAmbientBuffer()
{
	Format = DXGI_FORMAT_R16_UNORM;	// r16 存储环境光(环境光是一个单通道的值, 强度)
}

FAmbientBuffer::~FAmbientBuffer()
{
}

void FAmbientBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FRenderBuffer::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FAmbientBuffer::SetBufferSize(int wid, int hei)
{
	FRenderBuffer::SetBufferSize(wid, hei);
}

void FAmbientBuffer::PreDraw(float DeltaTime)
{
	FRenderBuffer::PreDraw(DeltaTime);
}

void FAmbientBuffer::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FRenderBuffer::UpdateCalculations(delta_time, viewport_info);
}

void FAmbientBuffer::Build(const XMFLOAT3& center)
{
	FRenderBuffer::Build(center);
}

void FAmbientBuffer::Draw(float deltaTime)
{
	FRenderBuffer::Draw(deltaTime);
}

void FAmbientBuffer::ResetView(int wid, int hei)
{
	FRenderBuffer::ResetView(wid, hei);
}

void FAmbientBuffer::BuildDescriptor()
{
	CalculateSRVOffset();
}

void FAmbientBuffer::BuildRenderTargetRTVOffset()
{
	CalculateRTVOffset();
}

void FAmbientBuffer::BuildSRVDescriptor()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};	// 着色器资源视图描述符
	srvDesc.Format = Format;	// 数据格式
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 视图维度-2D纹理
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;	// 着色器4分量映射

	srvDesc.Texture2D.MostDetailedMip = 0;	// 最详细的mipmap
	srvDesc.Texture2D.MipLevels = 1;	// mipmap级别
	srvDesc.Texture2D.PlaneSlice = 0;	// 纹理平面切片

	GetD3dDevice()->CreateShaderResourceView(RenderTarget->GetRenderTarget(), &srvDesc, RenderTarget->GetCPUShaderResourceView());
}

void FAmbientBuffer::BuildRTVDescriptor()
{
	if (FBufferRenderTarget* renderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};	// 渲染目标视图描述符
		rtvDesc.Format = Format;	// 数据格式
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// 视图维度-2D纹理
		rtvDesc.Texture2D.MipSlice = 0;	// 多级渐远纹理的第0级mipmap
		rtvDesc.Texture2D.PlaneSlice = 0;	// 纹理平面切片

		GetD3dDevice()->CreateRenderTargetView(renderTarget->GetRenderTarget(), &rtvDesc, renderTarget->GetCPURenderTargetView());
	}
}

void FAmbientBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
	// 资源描述
	CD3DX12_RESOURCE_DESC ResourceDesc;

	memset(&ResourceDesc, 0, sizeof(ResourceDesc));

	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;		// 资源维度2D纹理
	ResourceDesc.Alignment = 0;		// 对齐方式
	ResourceDesc.Width = Width;		// 资源宽度
	ResourceDesc.Height = Height;	// 资源高度
	ResourceDesc.DepthOrArraySize = 1;		// 资源数量
	ResourceDesc.MipLevels = 1;		// mipmap
	ResourceDesc.Format = Format;		// 数据格式
	ResourceDesc.SampleDesc.Count = 1;	// 采样描述-数量-1
	ResourceDesc.SampleDesc.Quality = 0;	// 采样描述-质量-0
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// 纹理布局方式-未知布局（由驱动自行决定）
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // 资源标记-标记资源可以被允许设置为我们的渲染目标（renderTarget）

	// 创建默认堆
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	const float AmbientColor[] = { 1.f, 1.f, 1.f, 1.f };
	CD3DX12_CLEAR_VALUE ClearValue(Format, AmbientColor);	// 清除值

	// 创建提交资源
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&ClearValue,
		IID_PPV_ARGS(OutResource.GetAddressOf())
	));
}
