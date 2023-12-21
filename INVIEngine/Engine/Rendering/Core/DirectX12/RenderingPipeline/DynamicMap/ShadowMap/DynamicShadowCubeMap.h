#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/Core/DynamicMap.h"

class GClientViewPort;

class FDynamicShadowCubeMap:public FDynamicMap
{
public:
	FDynamicShadowCubeMap();
	~FDynamicShadowCubeMap() override;
	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer) override;
	void PreDraw(float DeltaTime) override;
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info) override;
	void Build(const XMFLOAT3& center) override;
	void Draw(float deltaTime) override;
	void ResetView(int wid, int hei) override;

private:
	std::vector<GClientViewPort*> CubeMapViewPorts{};		// CubeMapÊÓ¿Ú 6¸ö
};

