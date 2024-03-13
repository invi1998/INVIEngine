#include "EngineMinimal.h"
#include "NormalBuffer.h"

#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

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
	if (FBufferRenderTarget* renderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		auto RenderTargetViewport = renderTarget->GetViewport();		// 获取视口
		auto RenderTargetScissorRect = renderTarget->GetScissorRect();	// 获取裁剪矩形

		GetD3dGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewport);	// 设置视口
		GetD3dGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);	// 设置裁剪矩形

		// 将资源状态从可读切换到渲染目标
		CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);

		// 清除渲染目标视图（清空画布）
		GetD3dGraphicsCommandList()->ClearRenderTargetView(
			renderTarget->GetCPURenderTargetView(),
			Colors::Blue,
			0,
			nullptr);

		// 获取深度模板(这里我们不需要做偏移，因为的深度模板是按照主视口做偏移的，我们获取最原始的深度模板就行）
		auto DepthStenciView = GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();

		// 清除深度模板缓冲区
		GetD3dGraphicsCommandList()->ClearDepthStencilView(
			DepthStenciView,
			D3D12_CLEAR_FLAG_DEPTH|D3D12_CLEAR_FLAG_STENCIL,
			1.f,
			0,
			0,
			nullptr
		);

		// 渲染主视口
		GeometryMap->DrawViewport(deltaTime);

		// 设置PSO
		RenderLayers->ResetPSO(RENDER_LAYER_SCREEN_NORMAL);

		// 设置深度模板(将我们的深度模板设置为渲染目标)
		GetD3dGraphicsCommandList()->OMSetRenderTargets(
			1,
			&renderTarget->GetCPURenderTargetView(),
			true,
			&DepthStenciView
		);

		// 渲染模型（执行shader）
		RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_OPAQUE, ERenderCondition::RC_Shadow);	// 渲染不透明物体
		RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_TRANSPARENT, ERenderCondition::RC_Shadow);	// 渲染透明物体
		RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_OPAQUE_REFLECT, ERenderCondition::RC_Shadow);	// 渲染反射物体
		

		// 将资源状态还原回可读状态
		CD3DX12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_GENERIC_READ
			
		);

	}
}

void FNormalBuffer::ResetView(int wid, int hei)
{
	FRenderBuffer::ResetView(wid, hei);
}

void FNormalBuffer::BuildDescriptor()
{
}

void FNormalBuffer::BuildRenderTargetRTVOffset()
{
	UINT rtvDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// 获取RTV描述符大小

	D3D12_CPU_DESCRIPTOR_HANDLE rtvDesHandle = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();	// 获取RTV描述符句柄

	int offset = FEngineRenderConfig::GetRenderConfig()->SwapChainCount +	// 获取偏移量 交换链
		6 +	// 反射的CubeMap
		6;	// shadowCubeMap 6个面 (点光源阴影）

	if (FBufferRenderTarget* renderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		renderTarget->GetCPURenderTargetView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvDesHandle, offset, rtvDescriptorSize);	// 设置RTV描述符句柄，将RTV描述符句柄指向RTV堆的指定偏移位置
	}
}

void FNormalBuffer::BuildSRVDescriptor()
{
}

void FNormalBuffer::BuildRTVDescriptor()
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

void FNormalBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
	OutResource->SetName(L"NormalBuffer");
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
