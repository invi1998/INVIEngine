#include "EngineMinimal.h"
#include "DynamicShadowCubeMap.h"

#include "Component/Light/Core/LightComponent.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Manage/LightManager.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/Core/RenderTarget.h"


FDynamicShadowCubeMap::FDynamicShadowCubeMap()
{
	CreateRenderTarget<FCubeMapRenderTarget>();
}

FDynamicShadowCubeMap::~FDynamicShadowCubeMap()
{
}

FDynamicShadowCubeMap::FTempViewportCapture::FTempViewportCapture(const XMFLOAT3& InCenterPoint)
{
	BuildViewportCapture(InCenterPoint);
}

void FDynamicShadowCubeMap::FTempViewportCapture::BuildViewportCapture(const XMFLOAT3& InCenterPoint)
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

void FDynamicShadowCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicShadowCubeMap::PreDraw(float DeltaTime)
{
	FDynamicMap::PreDraw(DeltaTime);

	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		for (int j = 0; j < GetLightManger()->GetLights().size(); j++)
		{
			CLightComponent* tmpLight = GetLightManger()->GetLights()[j];
			if (tmpLight->GetLightType() != ELightType::PointLight)
			{
				break;
			}
			// 指向哪个资源，转换器状态，因为我们有两个buffer，他两在不断交换
			CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
				inRenderTarget->GetRenderTarget(),				// 当前buffer缓冲区
				D3D12_RESOURCE_STATE_GENERIC_READ,		// 当前状态 表示资源即将被用作呈现目标
				D3D12_RESOURCE_STATE_RENDER_TARGET	// 目标状态 这将使资源可以用作渲染目标，并允许您对该资源执行呈现操作（写入状态）
			);
			GetD3dGraphicsCommandList()->ResourceBarrier(
				1,
				&ResourceBarrierPresent
			);

			D3D12_VIEWPORT Viewport = inRenderTarget->GetViewport();
			D3D12_RECT Rect = inRenderTarget->GetScissorRect();

			// 重置（更新）视口信息，裁剪矩阵信息
			GetD3dGraphicsCommandList()->RSSetViewports(1, &Viewport);
			GetD3dGraphicsCommandList()->RSSetScissorRects(1, &Rect);

			const UINT CBVOffsetSize = GeometryMap->GetViewportConstantBufferViews().GetConstantBufferByteSize();

			for (size_t i = 0; i < 6; i++)
			{
				// 清除画布
				constexpr float ColorBG[] = { 0.1f, 0.105f, 0.11f, 1.0f };
				GetD3dGraphicsCommandList()->ClearRenderTargetView(
					inRenderTarget->GetCPURenderTargetView()[i],		// 要清除的渲染目标视图
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
					&inRenderTarget->GetCPURenderTargetView()[i],		// 指定渲染目标
					true,								// true表明我们传入的句柄是一个内存连续的描述符指针
					&DSVCubeMapCPUDesc		// 传入深度
				);

				// 更新并且绑定6个视口
				auto ViewportAddress = GeometryMap->GetViewportConstantBufferViewsGPUVirtualAddr();
				// 1 主摄像机，i RenderTarget的cubeMap视口，j 是我们场景里的动态反射的组件，每个组件有6个摄像机
				ViewportAddress += (1 + i + j * 6) * CBVOffsetSize;
				GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddress);

				// 设置PSO
				RenderLayers->ResetPSO(RENDER_LAYER_OPAQUE_SHADOW, EPipelineState::ViewtianeShadow);

				// 渲染层级，对象模型
				RenderLayers->DrawMesh(DeltaTime, RENDER_LAYER_OPAQUE_REFLECT, ERenderCondition::RC_Shadow);
				RenderLayers->DrawMesh(DeltaTime, RENDER_LAYER_OPAQUE, ERenderCondition::RC_Shadow);
				RenderLayers->DrawMesh(DeltaTime, RENDER_LAYER_TRANSPARENT, ERenderCondition::RC_Shadow);
			}



			// 设置当前交换链buffer状态
			CD3DX12_RESOURCE_BARRIER ResourceBarrierRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
				inRenderTarget->GetRenderTarget(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_GENERIC_READ
			);

			
			GetD3dGraphicsCommandList()->ResourceBarrier(
				1,
				&ResourceBarrierRenderTarget
			);

			// 将渲染内容更新到CubeMap
			GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, inRenderTarget->GetGPUShaderResourceView());	// 6是背景天空球（为了观察渲染结果，将深度CubeMap信息更新到天空球观察）
			GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(8, inRenderTarget->GetGPUShaderResourceView());	// 8是阴影的cubeMap



		}
	}
}

void FDynamicShadowCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicMap::UpdateCalculations(delta_time, viewport_info);
}

void FDynamicShadowCubeMap::Build(const XMFLOAT3& center)
{
	FDynamicMap::Build(center);
}

void FDynamicShadowCubeMap::Draw(float deltaTime)
{
	FDynamicMap::Draw(deltaTime);
}

void FDynamicShadowCubeMap::ResetView(int wid, int hei)
{
	FDynamicMap::ResetView(wid, hei);
}

void FDynamicShadowCubeMap::BuildViewPort(const XMFLOAT3& InCenterPoint)
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

void FDynamicShadowCubeMap::BuildDepthStencil()
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

void FDynamicShadowCubeMap::BuildDepthStencilDescriptor()
{
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVCubeMapCPUDesc = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),	// DSV的起始地址
		1,	// 偏移1 第一个是给主渲染目标用的(场景）后面的才是给cubeMap用的
		DescriptorHandleIncrementSize	// DSV偏移量
	);
}

void FDynamicShadowCubeMap::BuildCubeMapRenderTargetDescriptor()
{
	BuildRenderTargetRTV();	// 视图
	BuildRenderTargetSRV();	// shader

	RenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void FDynamicShadowCubeMap::SetViewportPosition(const XMFLOAT3& position)
{
	FTempViewportCapture capture{ position };

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts[i]->SetPosition(position);
		CubeMapViewPorts[i]->FaceTarget(position, capture.TargetPoint[i], capture.Up[i]);
		CubeMapViewPorts[i]->BuildViewMatrix();
	}
}

bool FDynamicShadowCubeMap::IsExitDynamicReflectionMesh()
{
	return GeometryMap->GetDynamicViewportNum() > 0;
}

void FDynamicShadowCubeMap::BuildRenderTargetRTV()
{
	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT RTVSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		auto RTVStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
		// 为CubeMap创建渲染目标视图
		for (size_t i = 0; i < 6; i++)
		{
			inRenderTarget->CPURenderTargetView[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				RTVStart,		// RTV的起始地址
				FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,	// 交换链 前面的是给主渲染目标用的(场景）后面的才是给cubeMap用的
				RTVSize	// RTV偏移量
			);
		}
	}
}

void FDynamicShadowCubeMap::BuildRenderTargetSRV()
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
