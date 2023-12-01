#pragma once

#include "Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/Core/DynamicMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"

struct FViewportInfo;
class FRenderLayerManage;
struct FDirectXPipelineState;
struct FGeometryMap;
class GClientViewPort;

// ��̬CubeMap
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
	virtual void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer);
	virtual void PreDraw(float DeltaTime);
	virtual void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);
	virtual void Build(const XMFLOAT3& center);
	virtual void Draw(float deltaTime);

	// �����ӿ�
	virtual void BuildViewPort(const XMFLOAT3& CenterPoint);

	// �������ģ��
	virtual void BuildDepthStencil();

	virtual void BuildDepthStencilDescriptor();
	virtual void BuildCubeMapRenderTargetDescriptor();

	void SetCubeMapViewportPosition(const XMFLOAT3& position);

	// �жϳ������Ƿ���ڶ�̬����
	bool IsExitDynamicReflectionMesh();

protected:
	virtual void BuildRenderTargetRTV();
	virtual void BuildRenderTargetSRV();

protected:
	std::unique_ptr<FCubeMapRenderTarget> CubeMapRenderTarget{};	// renderTarget ��ȾĿ��
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVCubeMapCPUDesc;			// BSV���� ���ģ������

	std::vector<GClientViewPort*> CubeMapViewPorts{};		// CubeMap�ӿ� 6��

	FGeometryMap* GeometryMap = nullptr;	// ����
	FDirectXPipelineState* DirectXPipelineState = nullptr;	// ��Ⱦ����״̬����

	FRenderLayerManage* RenderLayers = nullptr;	// ��Ⱦ��

	ComPtr<ID3D12Resource> DepthStencilBuffer;		// ���ģ�建����
	UINT Width = 256;										// ���
	UINT Height = 256;										// �߶�
};
