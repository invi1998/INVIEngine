#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/Core/DynamicMap.h"

class GClientViewPort;

class FDynamicShadowCubeMap:public FDynamicMap
{
	struct FTempViewportCapture
	{
		FTempViewportCapture(const XMFLOAT3& InCenterPoint);

		XMFLOAT3 TargetPoint[6]{};
		XMFLOAT3 Up[6]{};

		void BuildViewportCapture(const XMFLOAT3& InCenterPoint);
	};

public:
	FDynamicShadowCubeMap();
	~FDynamicShadowCubeMap() override;
	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer) override;
	void PreDraw(float DeltaTime) override;
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info) override;
	void Build(const XMFLOAT3& center) override;
	void Draw(float deltaTime) override;
	void ResetView(int wid, int hei) override;

	// �����ӿ�
	virtual void BuildViewPort(const XMFLOAT3& CenterPoint);

	// �������ģ��
	virtual void BuildDepthStencil();

	virtual void BuildDepthStencilDescriptor();
	virtual void BuildCubeMapRenderTargetDescriptor();

	void SetViewportPosition(const XMFLOAT3& position);

	// �жϳ������Ƿ���ڶ�̬����
	bool IsExitDynamicReflectionMesh();


protected:
	virtual void BuildRenderTargetRTV();
	virtual void BuildRenderTargetSRV();

private:
	std::vector<GClientViewPort*> CubeMapViewPorts{};		// CubeMap�ӿ� 6��

	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVCubeMapCPUDesc;			// BSV���� ���ģ������

	ComPtr<ID3D12Resource> DepthStencilBuffer;		// ���ģ�建����
};

