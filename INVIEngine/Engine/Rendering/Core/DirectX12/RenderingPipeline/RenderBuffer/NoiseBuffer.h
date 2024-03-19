#pragma once
#include "Core/RenderBuffer.h"

// ���ͼ
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

	void BuildDescriptor();		// ����������
	void BuildRenderTargetRTVOffset();	// ������ȾĿ��RTV�ĵ�ַƫ��
	void BuildSRVDescriptor();	// ����SRV������
	void BuildRTVDescriptor();	// ����RTV������

protected:
	void BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource) override;
	void BuildUploadBuffer(const D3D12_RESOURCE_DESC& ResourceDesc, ComPtr<ID3D12Resource>& OutResource);	// �����ϴ��ѣ����������Դ���������湹���õ�����ͼ�������Դ
	D3D12_SUBRESOURCE_DATA GetSubResourceData(XMCOLOR* InBuffer, int w, int h);	// ��ȡ����Դ����

protected:
	ComPtr<ID3D12Resource> NoiseMapUploadBuffer;	// ����ͼ�ϴ���
};

