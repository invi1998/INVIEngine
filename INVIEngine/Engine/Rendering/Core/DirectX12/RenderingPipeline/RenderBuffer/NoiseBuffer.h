#pragma once
#include "Core/RenderBuffer.h"

// 噪点图
class FNoiseBuffer : public FRenderBuffer
{
public:
	FNoiseBuffer();
	~FNoiseBuffer() override;
	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
		FRenderLayerManage* inRenderLayer) override;
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
	void BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource) override;
	void BuildUploadBuffer(const D3D12_RESOURCE_DESC& ResourceDesc, ComPtr<ID3D12Resource>& OutResource);	// 构建上传堆，这里这个资源是我们上面构建好的噪声图传入的资源
	D3D12_SUBRESOURCE_DATA GetSubResourceData(XMCOLOR* InBuffer, int w, int h);	// 获取子资源数据

protected:
	ComPtr<ID3D12Resource> NoiseMapUploadBuffer;	// 噪声图上传堆
};

