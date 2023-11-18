#pragma once
#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"

struct FViewportInfo;
class FRenderLayerManage;
struct FDirectXPipelineState;
struct FGeometryMap;
class GClientViewPort;

// 动态CubeMap
class FDynamicCubeMap : public IDirectXDeviceInterface
{
public:
	FDynamicCubeMap();
	virtual void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer);
	virtual void PreDraw(float DeltaTime);
	virtual void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);
	virtual void Build(const XMFLOAT3& center);

protected:
	// 构建视口
	virtual void BuildViewPort(const XMFLOAT3& CenterPoint);

	// 构建深度模板
	virtual void BuildDepthStencil();

	virtual void BuildDepthStencilDescriptor();
	virtual void BuildCubeMapRenderTargetDescriptor();

	virtual void BuildRenderTargetRTV();
	virtual void BuildRenderTargetSRV();

protected:
	std::unique_ptr<FCubeMapRenderTarget> CubeMapRenderTarget{};	// renderTarget 渲染目标
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVCubeMapCPUDesc;			// BSV描述 深度模板描述

	std::vector<GClientViewPort*> CubeMapViewPorts{};		// CubeMap视口 6个

	FGeometryMap* GeometryMap = nullptr;	// 几何
	FDirectXPipelineState* DirectXPipelineState = nullptr;	// 渲染管线状态对象

	FRenderLayerManage* RenderLayers = nullptr;	// 渲染层

	ComPtr<ID3D12Resource> DepthStencilBuffer;		// 深度模板缓冲区
	UINT Width = 0;										// 宽度
	UINT Height = 0;									// 高度
};
