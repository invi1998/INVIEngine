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

	void BuildDescriptor();		// ����������
	void BuildRenderTargetRTV();	// ������ȾĿ��RTV
	void BuildSRVDescriptor();	// ����SRV������
	void BuildRTVDescriptor();	// ����RTV������

protected:
	void BuildRenderTargetMap(ComPtr<ID3D12Resource>& OutResource);	// ������ȾĿ��ͼ
};

