#include "EngineMinimal.h"
#include "DynamicCubeMap.h"

#include "Config/EngineRenderConfig.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"

FTempViewportCapture::FTempViewportCapture(const XMFLOAT3& InCenterPoint)
{
	BuildViewportCapture(InCenterPoint);
}

void FTempViewportCapture::BuildViewportCapture(const XMFLOAT3& InCenterPoint)
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
	CreateRenderTarget<FCubeMapRenderTarget>();
}

FDynamicCubeMap::~FDynamicCubeMap()
{
}

void FDynamicCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicCubeMap::PreDraw(float DeltaTime)
{
	FDynamicMap::PreDraw(DeltaTime);
}

void FDynamicCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicMap::UpdateCalculations(delta_time, viewport_info);
}

void FDynamicCubeMap::Build(const XMFLOAT3& center)
{
	FDynamicMap::Build(center);
}

void FDynamicCubeMap::Draw(float deltaTime)
{
	FDynamicMap::Draw(deltaTime);

	// 更新CubeMap
	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(RenderTarget.get()))
	{
		GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, inRenderTarget->GPUShaderResourceView);
	}
}

void FDynamicCubeMap::ResetView(int wid, int hei)
{
	FDynamicMap::ResetView(wid, hei);
}

void FDynamicCubeMap::BuildViewPort(const XMFLOAT3& CenterPoint)
{
	FTempViewportCapture capture{ CenterPoint };

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts.push_back(CreateObject<GClientViewPort>(new GClientViewPort()));
		GClientViewPort* viewport = CubeMapViewPorts[CubeMapViewPorts.size() - 1];

		viewport->SetPosition(CenterPoint);
		viewport->FaceTarget(CenterPoint, capture.TargetPoint[i], capture.Up[i]);
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

	RenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void FDynamicCubeMap::SetViewportPosition(const XMFLOAT3& position)
{
	FTempViewportCapture capture{ position };

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts[i]->SetPosition(position);
		CubeMapViewPorts[i]->FaceTarget(position, capture.TargetPoint[i], capture.Up[i]);
		CubeMapViewPorts[i]->BuildViewMatrix();
	}
}

void FDynamicCubeMap::BuildRenderTargetSRV()
{
	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT CBVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

		// 为CubeMap创建CPU shader资源视图
		inRenderTarget->CPUShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			CPUSRVDesHeapStart,		// CPU SRV的起始地址
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount(),	// 偏移量
			CBVDescriptorSize	// SRV偏移量
		);

		// 为CubeMap创建GPU shader资源视图
		inRenderTarget->GPUShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			GPUSRVDesHeapStart,		// GPU SRV的起始地址
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount(),	// 偏移量
			CBVDescriptorSize	// SRV偏移量
		);
	}
}
