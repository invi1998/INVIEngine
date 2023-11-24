#include "EngineMinimal.h"
#include "DynamicCubeMap.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"


FDynamicCubeMap::FTempViewportCapture::FTempViewportCapture(const XMFLOAT3& InCenterPoint)
{
	BuildViewportCapture(InCenterPoint);
}

void FDynamicCubeMap::FTempViewportCapture::BuildViewportCapture(const XMFLOAT3& InCenterPoint)
{
	// +x, -x, +y, -y, +z, -z
	// 捕获摄像机的6个面
	TargetPoint[0] = XMFLOAT3{ InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z };
	TargetPoint[1] = XMFLOAT3{ InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z };
	TargetPoint[2] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z };
	TargetPoint[3] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z };
	TargetPoint[4] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f };
	TargetPoint[5] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f };


	Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f };
	Up[1] = XMFLOAT3{ 0.f, 1.f, 0.f };
	Up[2] = XMFLOAT3{ 0.f, 0.f, -1.f };
	Up[3] = XMFLOAT3{ 0.f, 0.f, 1.f };
	Up[4] = XMFLOAT3{ 0.f, 1.f, 0.f };
	Up[5] = XMFLOAT3{ 0.f, 1.f, 0.f };
}

FDynamicCubeMap::FDynamicCubeMap()
{
	CubeMapRenderTarget = std::make_unique<FCubeMapRenderTarget>();
}

void FDynamicCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	GeometryMap = inGeometryMap;
	DirectXPipelineState = inDirectXPipelineState;
	RenderLayers = inRenderLayer;
}

void FDynamicCubeMap::PreDraw(float DeltaTime)
{
	// 渲染灯光，材质，贴图
	GeometryMap->Draw(DeltaTime);

	// 指向哪个资源，转换器状态，因为我们有两个buffer，他两在不断交换
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		CubeMapRenderTarget->GetRenderTarget(),				// 当前buffer缓冲区
		D3D12_RESOURCE_STATE_GENERIC_READ,		// 当前状态 表示资源即将被用作呈现目标
		D3D12_RESOURCE_STATE_RENDER_TARGET	// 目标状态 这将使资源可以用作渲染目标，并允许您对该资源执行呈现操作（写入状态）
	);
	GetD3dGraphicsCommandList()->ResourceBarrier(
		1,
		&ResourceBarrierPresent
	);

	D3D12_VIEWPORT Viewport = CubeMapRenderTarget->GetViewport();
	D3D12_RECT Rect = CubeMapRenderTarget->GetScissorRect();

	// 重置（更新）视口信息，裁剪矩阵信息
	GetD3dGraphicsCommandList()->RSSetViewports(1, &Viewport);
	GetD3dGraphicsCommandList()->RSSetScissorRects(1, &Rect);

	const UINT CBVOffsetSize = GeometryMap->ViewportConstantBufferViews.GetConstantBufferByteSize();

	for (size_t i = 0; i < 6; i++)
	{
		// 清除画布
		constexpr float ColorBG[] = { 0.1f, 0.105f, 0.11f, 1.0f };
		GetD3dGraphicsCommandList()->ClearRenderTargetView(
			CubeMapRenderTarget->CPURenderTargetView[i],		// 要清除的渲染目标视图
			ColorBG,		// 画布颜色
			0,		// 后面这两个参数是和视口相关的，这里不在这里设置，后面会有专门的设置方法
			nullptr
		);

		// 清除深度和模板缓冲区
		GetD3dGraphicsCommandList()->ClearDepthStencilView(
			DSVCubeMapCPUDesc,	// 传入CPU内存（要清除的深度模板缓冲区内存）
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,		// 清除深度和模板缓冲区
			1.f,	// 用1来清除我们的深度缓冲区（将深度缓冲区设置为1
			0,		// 用0来清除我们的模板缓冲区（模板缓冲区设置为0
			0,
			nullptr
		);

		// 指定渲染缓冲区（输出合并阶段）设置渲染目标视图
		GetD3dGraphicsCommandList()->OMSetRenderTargets(
			1,									// 指定渲染目标数 1
			&CubeMapRenderTarget->CPURenderTargetView[i],		// 指定渲染目标
			true,								// true表明我们传入的句柄是一个内存连续的描述符指针
			&DSVCubeMapCPUDesc		// 传入深度
		);

		// 更新并且绑定6个视口
		auto ViewportAddress = GeometryMap->ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
		ViewportAddress += (i + 1) * CBVOffsetSize;
		GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddress);

		// 渲染层级，对象模型
		RenderLayers->Draw(RENDER_LAYER_BACKGROUND, DeltaTime);
		RenderLayers->Draw(RENDER_LAYER_OPAQUE, DeltaTime);
		RenderLayers->Draw(RENDER_LAYER_TRANSPARENT, DeltaTime);
	}

	

	// 设置当前交换链buffer状态
	CD3DX12_RESOURCE_BARRIER ResourceBarrierRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		CubeMapRenderTarget->GetRenderTarget(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_GENERIC_READ
	);
	GetD3dGraphicsCommandList()->ResourceBarrier(
		1,
		&ResourceBarrierRenderTarget
	);

}

void FDynamicCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{

	if (CubeMapViewPorts.size() == 6)
	{
		for (int k = 0; k < GeometryMap->DynamicReflectionMeshComponents.size(); k++)
		{
			auto meshComponent = GeometryMap->DynamicReflectionMeshComponents[k];
			XMFLOAT3 position = meshComponent->GetPosition();

			SetCubeMapViewportPosition(position);
			// 更新视口
			for (size_t i = 0; i < 6; i++)
			{
				FViewportInfo tempViewport{};
				XMFLOAT3 position = CubeMapViewPorts[i]->GetPosition();
				tempViewport.CameraPosition = XMFLOAT4{ position.x, position.y, position.z, 1.f};
				tempViewport.ViewMatrix = CubeMapViewPorts[i]->GetViewMatrix();
				tempViewport.ProjectionMatrix = CubeMapViewPorts[i]->GetProjectionMatrix();

				GeometryMap->UpdateCalculationViewport(tempViewport, 
					i + k*6	// 动态反射的6个摄像机
					+1);
			}
		}
		
	}
	
}

void FDynamicCubeMap::Build(const XMFLOAT3& center)
{
	BuildViewPort(center);		// 构建视口

	BuildCubeMapRenderTargetDescriptor();		// 构建CubeMap渲染目标描述

	BuildDepthStencilDescriptor();		// 构建深度模板描述
	
	BuildDepthStencil();		// 构建深度模板
}

void FDynamicCubeMap::Draw(float deltaTime)
{
	// 更新CubeMap
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, CubeMapRenderTarget->GPUShaderResourceView);
}

void FDynamicCubeMap::BuildViewPort(const XMFLOAT3& InCenterPoint)
{
	FTempViewportCapture capture{ InCenterPoint };
	

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts.push_back(CreateObject<GClientViewPort>(new GClientViewPort()));
		GClientViewPort* viewport = CubeMapViewPorts[CubeMapViewPorts.size() - 1];

		viewport->SetPosition(InCenterPoint);
		viewport->FaceTarget(InCenterPoint, capture.TargetPoint[i], capture.Up[i]);
		viewport->SetFrustum(XM_PIDIV2, 1, 1, 0.1f, 10000.f);
		viewport->BuildViewMatrix();
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = Width;
	ResourceDesc.Height = Height;
	ResourceDesc.Alignment = 0;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	GetD3dDevice()->CreateCommittedResource(
		&Properties,
		D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON, &ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	GetD3dDevice()->CreateDepthStencilView(DepthStencilBuffer.Get(), nullptr, DSVCubeMapCPUDesc);

	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	GetD3dGraphicsCommandList()->ResourceBarrier(1, &Barrier);
}

void FDynamicCubeMap::BuildDepthStencilDescriptor()
{
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVCubeMapCPUDesc = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),	// DSV的起始地址
		1,	// 偏移1 第一个是给主渲染目标用的(场景）后面的才是给cubeMap用的
		DescriptorHandleIncrementSize	// DSV偏移量
	);
}

void FDynamicCubeMap::BuildCubeMapRenderTargetDescriptor()
{

	BuildRenderTargetRTV();	// 视图
	BuildRenderTargetSRV();	// shader

	CubeMapRenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);
	
}

void FDynamicCubeMap::SetCubeMapViewportPosition(const XMFLOAT3& position)
{
	
	FTempViewportCapture capture{ position };

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts[i]->SetPosition(position);
		CubeMapViewPorts[i]->FaceTarget(position, capture.TargetPoint[i], capture.Up[i]);
		CubeMapViewPorts[i]->BuildViewMatrix();
	}
}

void FDynamicCubeMap::BuildRenderTargetRTV()
{
	UINT RTVSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	auto RTVStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
	// 为CubeMap创建渲染目标视图
	for (size_t i = 0; i < 6; i++)
	{
		CubeMapRenderTarget->CPURenderTargetView[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			RTVStart,		// RTV的起始地址
			FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,	// 交换链 前面的是给主渲染目标用的(场景）后面的才是给cubeMap用的
			RTVSize	// RTV偏移量
		);
	}
}

void FDynamicCubeMap::BuildRenderTargetSRV()
{
	UINT CBVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

	// 为CubeMap创建CPU shader资源视图
	CubeMapRenderTarget->CPUShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		CPUSRVDesHeapStart,		// CPU SRV的起始地址
		GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount(),	// 偏移量
		CBVDescriptorSize	// SRV偏移量
	);

	// 为CubeMap创建GPU shader资源视图
	CubeMapRenderTarget->GPUShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		GPUSRVDesHeapStart,		// GPU SRV的起始地址
		GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount(),	// 偏移量
		CBVDescriptorSize	// SRV偏移量
	);
}
