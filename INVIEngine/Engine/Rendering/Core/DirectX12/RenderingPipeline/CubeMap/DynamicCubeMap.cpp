#include "EngineMinimal.h"
#include "DynamicCubeMap.h"

#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"


FDynamicCubeMap::FDynamicCubeMap()
{
}

void FDynamicCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	GeometryMap = inGeometryMap;
	DirectXPipelineState = inDirectXPipelineState;
	RenderLayers = inRenderLayer;
}

void FDynamicCubeMap::Draw(float DeltaTime)
{
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

		// 渲染

		// 渲染灯光，材质，贴图
		GeometryMap->Draw(DeltaTime);
		// 渲染层级，对象模型
		RenderLayers->Draw(DeltaTime);
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
		// 更新视口
		for (size_t i = 0; i < 6; i++)
		{
			FViewportInfo tempViewport{};
			XMFLOAT3 position = CubeMapViewPorts[i]->GetPosition();
			tempViewport.CameraPosition = XMFLOAT4{ position.x, position.y, position.z, 1.f};
			tempViewport.ViewMatrix = CubeMapViewPorts[i]->GetViewMatrix();
			tempViewport.ProjectionMatrix = CubeMapViewPorts[i]->GetProjectionMatrix();

			GeometryMap->UpdateCalculationViewport(tempViewport, i+1);
		}
	}
	
}

void FDynamicCubeMap::BuildViewPort(const XMFLOAT3& InCenterPoint)
{
	struct FTempViewportCapture
	{
		XMFLOAT3 TargetPoint[6];
		XMFLOAT3 Up[6];
	};
	// +x, -x, +y, -y, +z, -z
	FTempViewportCapture Capture{};

	// 捕获摄像机的6个面
	Capture.TargetPoint[0] = XMFLOAT3{ InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z};
	Capture.TargetPoint[1] = XMFLOAT3{ InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z};
	Capture.TargetPoint[2] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z};
	Capture.TargetPoint[3] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z};
	Capture.TargetPoint[4] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f};
	Capture.TargetPoint[5] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f };

	
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 0.f, -1.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 0.f, 1.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f };

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts.push_back(CreateObject<GClientViewPort>(new GClientViewPort()));
		GClientViewPort* viewport = CubeMapViewPorts[CubeMapViewPorts.size() - 1];

		viewport->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.Up[i]);
		viewport->SetFrustum(XM_PIDIV2, 1, 1, 0.1f, 10000.f);
		viewport->BuildViewMatrix();
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
}
