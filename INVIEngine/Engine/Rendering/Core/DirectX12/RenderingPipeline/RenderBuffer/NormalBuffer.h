#pragma once
#include "Core/RenderBuffer.h"

class FNormalBuffer : public FRenderBuffer
{
public:
	FNormalBuffer();
	~FNormalBuffer() override;
	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer) override;
	void Init(int wid, int hei) override;
	void PreDraw(float DeltaTime) override;
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info) override;
	void Build(const XMFLOAT3& center) override;
	void Draw(float deltaTime) override;
	void ResetView(int wid, int hei) override;

	void BuildDescriptor();		// 构建描述符
	void BuildRenderTargetRTV();	// 构建渲染目标RTV
	void BuildSRVDescriptor();	// 构建SRV描述符
	void BuildRTVDescriptor();	// 构建RTV描述符

protected:
	void BuildRenderTargetMap(ComPtr<ID3D12Resource>& OutResource);	// 构建渲染目标图
};

