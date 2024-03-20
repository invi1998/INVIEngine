#pragma once
#include "Interface/DirectXDeviceInterface.h"

class FRenderTarget;
class FRenderLayerManage;
struct FDirectXPipelineState;
struct FGeometryMap;
struct FViewportInfo;

class FDynamicMap : public IDirectXDeviceInterface
{
public:
	FDynamicMap();
	~FDynamicMap() override = default;

	virtual void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer);
	virtual void SetBufferSize(int wid, int hei);
	virtual void PreDraw(float DeltaTime);
	virtual void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);
	virtual void Build(const XMFLOAT3& center);
	virtual void Draw(float deltaTime);
	virtual void ResetView(int wid, int hei);

	virtual void CalculateSRVOffset();
	virtual void CalculateRTVOffset();

	UINT GetWidth() const { return Width; }
	UINT GetHeight() const { return Height; }

	UINT GetSRVOffset() const { return SRVOffset; }
	UINT GetRTVOffset() const { return RTVOffset; }

	void SetSRVOffset(const UINT inSRVOffset) { SRVOffset = inSRVOffset; }
	void SetRTVOffset(const UINT inRTVOffset) { RTVOffset = inRTVOffset; }

	std::shared_ptr<FRenderTarget>& GetRenderTarget() { return RenderTarget; }

	template<typename T>
	void CreateRenderTarget()
	{
		if (!RenderTarget)
		{
			RenderTarget = std::make_shared<T>();
		}
	}

protected:
	std::shared_ptr<FRenderTarget> RenderTarget = nullptr;	// renderTarget ��ȾĿ��

	FGeometryMap* GeometryMap = nullptr;	// ����
	FDirectXPipelineState* DirectXPipelineState = nullptr;	// ��Ⱦ����״̬����

	FRenderLayerManage* RenderLayers = nullptr;	// ��Ⱦ��

	UINT Width = 256;										// ���
	UINT Height = 256;										// �߶�

	UINT SRVOffset = 0;	// SRVƫ�� ���ڰ󶨵������� ShaderResourceView
	UINT RTVOffset = 0;	// RTVƫ�� ���ڰ󶨵������� RenderTargetView

};

// Path: INVIEngine/Engine/Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/CubeMap/DynamicCubeMap.cpp
