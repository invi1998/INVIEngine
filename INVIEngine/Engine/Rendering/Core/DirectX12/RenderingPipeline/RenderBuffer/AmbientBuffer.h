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

	void BuildDescriptor();		// ����������
	void BuildRenderTargetRTVOffset();	// ������ȾĿ��RTV�ĵ�ַƫ��
	void BuildSRVDescriptor();	// ����SRV������
	void BuildRTVDescriptor();	// ����RTV������

protected:
	void BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource) override;	// ������ȾĿ��ͼ
};

