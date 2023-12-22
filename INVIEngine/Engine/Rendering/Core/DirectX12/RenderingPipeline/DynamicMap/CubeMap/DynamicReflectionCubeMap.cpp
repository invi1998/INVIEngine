#include "EngineMinimal.h"
#include "DynamicReflectionCubeMap.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"


FDynamicReflectionCubeMap::FDynamicReflectionCubeMap():FDynamicCubeMap()
{
}

void FDynamicReflectionCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	FDynamicCubeMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicReflectionCubeMap::PreDraw(float DeltaTime)
{
	FDynamicCubeMap::PreDraw(DeltaTime);

	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		for (int j = 0; j < GeometryMap->GetDynamicReflectionMeshComponents().size(); j++)
		{
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
				auto ViewportAddress = GeometryMap->GetViewportConstantBufferViews().GetBuffer()->GetGPUVirtualAddress();
				// 1 主摄像机，i RenderTarget的cubeMap视口，j 是我们场景里的动态反射的组件，每个组件有6个摄像机
				ViewportAddress += (1 + i + j * 6) * CBVOffsetSize;
				GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddress);

				// 渲染层级，对象模型
				RenderLayers->Draw(RENDER_LAYER_BACKGROUND, DeltaTime);
				RenderLayers->Draw(RENDER_LAYER_OPAQUE, DeltaTime);
				RenderLayers->Draw(RENDER_LAYER_TRANSPARENT, DeltaTime);
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

			// 准备主视口
			StartSetMainViewportRenderTarget();

			// 渲染主视口
			GeometryMap->DrawViewport(DeltaTime);

			Draw(DeltaTime);

			// 从渲染层级中针对指定的模型进行渲染（因为动态反射不是对所有的模型都进行动态反射计算的，只对动态反射材质的模型进行渲染）
			RenderLayers->FindObjectDraw(DeltaTime, RENDER_LAYER_OPAQUE_REFLECT, GeometryMap->GetDynamicReflectionMeshComponents()[j]);

			// 重置CubeMap
			GeometryMap->DrawCubeMapTexture(DeltaTime);

			// 结束当前主视口渲染
			EndSetMainViewportRenderTarget();
		}
	}
}

void FDynamicReflectionCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{

	if (CubeMapViewPorts.size() == 6)
	{
		for (int k = 0; k < GeometryMap->GetDynamicReflectionMeshComponents().size(); k++)
		{
			auto meshComponent = GeometryMap->GetDynamicReflectionMeshComponents()[k];
			XMFLOAT3 position = meshComponent->GetPosition();

			SetViewportPosition(position);
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

void FDynamicReflectionCubeMap::Build(const XMFLOAT3& center)
{
	BuildViewPort(center);		// 构建视口

	BuildCubeMapRenderTargetDescriptor();		// 构建CubeMap渲染目标描述

	BuildDepthStencilDescriptor();		// 构建深度模板描述

	BuildDepthStencil();		// 构建深度模板
}

void FDynamicReflectionCubeMap::BuildDepthStencilDescriptor()
{
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVCubeMapCPUDesc = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),	// DSV的起始地址
		1,	// 偏移1 第一个是给主渲染目标用的(场景）后面的才是给cubeMap用的
		DescriptorHandleIncrementSize	// DSV偏移量
	);
}

void FDynamicReflectionCubeMap::BuildRenderTargetRTV()
{
	FDynamicCubeMap::BuildRenderTargetRTV();

	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT RTVSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		auto RTVStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
		// 为CubeMap创建渲染目标视图
		for (size_t i = 0; i < 6; i++)
		{
			inRenderTarget->GetCPURenderTargetView()[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				RTVStart,		// RTV的起始地址
				FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,	// 交换链 前面的是给主渲染目标用的(场景）后面的才是给cubeMap用的
				RTVSize	// RTV偏移量
			);
		}
	}
}

bool FDynamicReflectionCubeMap::IsExitDynamicReflectionMesh()
{
	return GeometryMap->GetDynamicViewportNum() > 0;
}

