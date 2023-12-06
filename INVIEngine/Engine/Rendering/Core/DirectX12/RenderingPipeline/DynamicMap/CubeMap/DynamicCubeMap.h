#pragma once

#include "Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/Core/DynamicMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"

struct FViewportInfo;
class FRenderLayerManage;
struct FDirectXPipelineState;
struct FGeometryMap;
class GClientViewPort;

// 动态CubeMap
class FDynamicCubeMap : public FDynamicMap
{

	struct FTempViewportCapture
	{
		FTempViewportCapture(const XMFLOAT3& InCenterPoint);

		XMFLOAT3 TargetPoint[6]{};
		XMFLOAT3 Up[6]{};

		void BuildViewportCapture(const XMFLOAT3& InCenterPoint);
	};

public:
	FDynamicCubeMap();
	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer) override;
	void PreDraw(float DeltaTime) override;
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info) override;
	void Build(const XMFLOAT3& center) override;
	void Draw(float deltaTime) override;

	// 构建视口
	virtual void BuildViewPort(const XMFLOAT3& CenterPoint);

	// 构建深度模板
	virtual void BuildDepthStencil();

	virtual void BuildDepthStencilDescriptor();
	virtual void BuildCubeMapRenderTargetDescriptor();

	void SetViewportPosition(const XMFLOAT3& position);

	// 判断场景中是否存在动态反射
	bool IsExitDynamicReflectionMesh();

protected:
	virtual void BuildRenderTargetRTV();
	virtual void BuildRenderTargetSRV();

protected:
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVCubeMapCPUDesc;			// BSV描述 深度模板描述

	std::vector<GClientViewPort*> CubeMapViewPorts{};		// CubeMap视口 6个

	ComPtr<ID3D12Resource> DepthStencilBuffer;		// 深度模板缓冲区

};
