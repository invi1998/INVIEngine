#pragma once
#include "Core/RenderBuffer.h"

class FAmbientBuffer : public FRenderBuffer
{
public:
	FAmbientBuffer();
	~FAmbientBuffer() override;
	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer) override;
	void SetBufferSize(int wid, int hei) override;
	void PreDraw(float DeltaTime) override;
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info) override;
	void Build(const XMFLOAT3& center) override;
	void Draw(float deltaTime) override;
	void ResetView(int wid, int hei) override;

	void BuildDescriptor();		// 构建描述符
	void BuildRenderTargetRTVOffset();	// 构建渲染目标RTV的地址偏移
	void BuildSRVDescriptor();	// 构建SRV描述符
	void BuildRTVDescriptor();	// 构建RTV描述符

protected:
	void BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource) override;	// 构建渲染目标图
};

