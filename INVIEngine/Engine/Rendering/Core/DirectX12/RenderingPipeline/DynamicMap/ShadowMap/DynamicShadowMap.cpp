#include "EngineMinimal.h"
#include "DynamicShadowMap.h"

#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/ShadowMapRenderTarget.h"

FDynamicShadowMap::FDynamicShadowMap()
{
	CreateRenderTarget<FShadowMapRenderTarget>();
}

FDynamicShadowMap::~FDynamicShadowMap()
{
}

void FDynamicShadowMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicShadowMap::PreDraw(float DeltaTime)
{
	FDynamicMap::PreDraw(DeltaTime);
}

void FDynamicShadowMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicMap::UpdateCalculations(delta_time, viewport_info);
}

void FDynamicShadowMap::Build(const XMFLOAT3& center)
{
	FDynamicMap::Build(center);
}

void FDynamicShadowMap::Draw(float deltaTime)
{
	FDynamicMap::Draw(deltaTime);
}

void FDynamicShadowMap::SetViewportPosition(const XMFLOAT3& position)
{
	ShadowViewPort->SetPosition(position);
	BuildViewMatrix();
}

void FDynamicShadowMap::SetViewportRotation(const XMFLOAT3& rotation)
{
	ShadowViewPort->SetRotation(fvector_3d(rotation.x, rotation.y, rotation.z));
	BuildViewMatrix();
}

void FDynamicShadowMap::BuildViewPort(const XMFLOAT3& position)
{
	ShadowViewPort = CreateObject<GClientViewPort>(new GClientViewPort());
	ShadowViewPort->SetPosition(position);
	ShadowViewPort->FaceTarget(position, { 10.f, 0.f, 0.f });
	ShadowViewPort->SetFrustum(XM_PIDIV2, 1.f, 1.f, 0.1f, 10000.f);
	BuildViewMatrix();
}

void FDynamicShadowMap::BuildViewMatrix()
{
	ShadowViewPort->BuildViewMatrix();
}

void FDynamicShadowMap::BuildDepthStencilViewDesc()
{
	// 拿到DSV的描述符偏移值
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	if (FShadowMapRenderTarget* innerRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(RenderTarget.get()))
	{
		innerRenderTarget->CPUDepthStencilView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),		// 主视口DSV，在此基础上偏移
			1 + 1,													// 1个主视口DSV + 1个CubeMap的DSV，偏移过这两个之后，才是阴影的DSV
			DescriptorHandleIncrementSize);							// 描述符偏移值
	}
}

void FDynamicShadowMap::BuildShadowMapRenderTargetDescriptor()
{
	BuildRenderTargetSRV();

	RenderTarget->Init(Width, Height, DXGI_FORMAT_D24_UNORM_S8_UINT);
}

void FDynamicShadowMap::BuildRenderTargetSRV()
{
	if (FShadowMapRenderTarget* inRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT CBVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

		// 为CubeMap创建CPU shader资源视图
		inRenderTarget->CPUShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			CPUSRVDesHeapStart,		// CPU SRV的起始地址
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount() + 1,	// 偏移量 Texture2D + CubeMap + DynamicCubeMap
			CBVDescriptorSize	// SRV偏移量
		);

		// 为CubeMap创建GPU shader资源视图
		inRenderTarget->GPUShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			GPUSRVDesHeapStart,		// GPU SRV的起始地址
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount() + 1,	// 偏移量
			CBVDescriptorSize	// SRV偏移量
		);
	}
}
