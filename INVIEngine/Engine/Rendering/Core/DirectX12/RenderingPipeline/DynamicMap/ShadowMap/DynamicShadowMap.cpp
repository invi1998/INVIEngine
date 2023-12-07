#include "EngineMinimal.h"
#include "DynamicShadowMap.h"

#include "Component/Mesh/Core/MeshComponentType.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
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

	if (FShadowMapRenderTarget* innerRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(RenderTarget.get()))
	{
		// 设置资源状态为可写
		CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			innerRenderTarget->GetRenderTarget(),				// 资源
			D3D12_RESOURCE_STATE_GENERIC_READ,				// 资源状态 可读
			D3D12_RESOURCE_STATE_DEPTH_WRITE);		// 资源状态 转为 可写

		GetD3dGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier);

		// 需要每帧更新的数据 绑定矩形框
		auto RenderTargetViewPort = innerRenderTarget->GetViewport();
		auto RenderTargetScissorRect = innerRenderTarget->GetScissorRect();

		GetD3dGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewPort);
		GetD3dGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);

		UINT CBVDescriptorSize = GeometryMap->ViewportConstantBufferViews.GetConstantBufferByteSize();

		// 清除深度模板缓冲区
		GetD3dGraphicsCommandList()->ClearDepthStencilView(			
			innerRenderTarget->CPUDepthStencilView,		// DSV 描述符
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,					// 清除深度模板缓冲区
			1.0f,											// 深度值
			0,												// 模板值
			0,												// 清除区域数量
			nullptr);										// 清除区域)

		// 输出合并阴影贴图 （因为这里不考虑进行像素着色，所以不需要设置渲染目标（渲染目标设置为0））
		GetD3dGraphicsCommandList()->OMSetRenderTargets(
			0,				// 渲染目标数量
			nullptr,		// 偏移
			false,			// 是否采用单个句柄来绑定渲染目标视图和深度模板视图
			&innerRenderTarget->CPUDepthStencilView);		// 指定深度模板视图

		// 绑定摄像机
		auto ViewportAddr = GeometryMap->ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
		ViewportAddr += (1 + GeometryMap->GetDynamicViewportNum()) * CBVDescriptorSize;

		GetD3dGraphicsCommandList()->SetGraphicsRootShaderResourceView(		
				1,	// 根参数的起始索引
				ViewportAddr	// GPU资源视图
				);

		DrawShadowMapTexture(deltaTime);

		RenderLayers->ResetPSO(RENDER_LAYER_OPAQUE_SHADOW);

		RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_OPAQUE);

		// 将资源状态由可写转换为可读
		CD3DX12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
			innerRenderTarget->GetRenderTarget(),				// 资源
			D3D12_RESOURCE_STATE_DEPTH_WRITE,				// 资源状态 可写
			D3D12_RESOURCE_STATE_GENERIC_READ);		// 资源状态 转为 可读

		GetD3dGraphicsCommandList()->ResourceBarrier(1, &ResourceBarrier2);
	}
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

void FDynamicShadowMap::DrawShadowMapTexture(float DeltaTime)
{
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
			7,	// 根参数的起始索引
			RenderTarget->GetGPUShaderResourceView()	// GPU资源视图
		);
}

void FDynamicShadowMap::BuildRenderTargetSRV()
{
	if (FShadowMapRenderTarget* inRenderTarget = dynamic_cast<FShadowMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT CBVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

		// 为CubeMap创建CPU shader资源视图 主要是创建Shadow常量缓冲区
		inRenderTarget->CPUShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			CPUSRVDesHeapStart,		// CPU SRV的起始地址
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount() + 1,	// 偏移量 Texture2D + CubeMap + DynamicCubeMap
			CBVDescriptorSize	// SRV偏移量
		);

		// 为CubeMap创建GPU shader资源视图 后期我们渲染阴影贴图的时候，需要将这个资源视图绑定到着色器上
		inRenderTarget->GPUShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			GPUSRVDesHeapStart,		// GPU SRV的起始地址
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount() + 1,	// 偏移量
			CBVDescriptorSize	// SRV偏移量
		);
	}
}
