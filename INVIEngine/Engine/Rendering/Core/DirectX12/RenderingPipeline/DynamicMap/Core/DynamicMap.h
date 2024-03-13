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

	template<typename T>
	void CreateRenderTarget()
	{
		if (!RenderTarget)
		{
			RenderTarget = std::make_shared<T>();
		}
	}

protected:
	std::shared_ptr<FRenderTarget> RenderTarget = nullptr;	// renderTarget 渲染目标

	FGeometryMap* GeometryMap = nullptr;	// 几何
	FDirectXPipelineState* DirectXPipelineState = nullptr;	// 渲染管线状态对象

	FRenderLayerManage* RenderLayers = nullptr;	// 渲染层

	UINT Width = 256;										// 宽度
	UINT Height = 256;										// 高度

};

// Path: INVIEngine/Engine/Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/CubeMap/DynamicCubeMap.cpp
