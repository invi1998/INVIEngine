#pragma once
#include "DynamicMap.h"

class GClientViewPort;

struct FTempViewportCapture
{
	FTempViewportCapture(const XMFLOAT3& InCenterPoint);

	XMFLOAT3 TargetPoint[6]{};
	XMFLOAT3 Up[6]{};

	void BuildViewportCapture(const XMFLOAT3& InCenterPoint);
};

class FDynamicCubeMap : public FDynamicMap
{
public:
	FDynamicCubeMap();
	~FDynamicCubeMap() override;
	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
		FRenderLayerManage* inRenderLayer) override;
	void PreDraw(float DeltaTime) override;
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info) override;
	void Build(const XMFLOAT3& center) override;
	void Draw(float deltaTime) override;
	void ResetView(int wid, int hei) override;

	// 构建视口
	virtual void BuildViewPort(const XMFLOAT3& CenterPoint);

	// 构建深度模板
	virtual void BuildDepthStencil();

	virtual void BuildDepthStencilDescriptor();
	virtual void BuildCubeMapRenderTargetDescriptor();

	virtual void SetViewportPosition(const XMFLOAT3& position);

protected:
	virtual void BuildRenderTargetRTV() = 0;
	virtual void BuildRenderTargetSRV() = 0;

protected:

	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVCubeMapCPUDesc;			// BSV描述 深度模板描述

	std::vector<GClientViewPort*> CubeMapViewPorts{};		// CubeMap视口 6个

	ComPtr<ID3D12Resource> DepthStencilBuffer;		// 深度模板缓冲区

};

