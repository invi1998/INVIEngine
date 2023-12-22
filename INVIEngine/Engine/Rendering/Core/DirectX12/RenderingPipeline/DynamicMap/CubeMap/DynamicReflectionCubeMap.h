#pragma once

#include "Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/Core/DynamicCubeMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/Core/DynamicMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"

struct FViewportInfo;
class FRenderLayerManage;
struct FDirectXPipelineState;
struct FGeometryMap;
class GClientViewPort;

// ��̬����CubeMap
class FDynamicReflectionCubeMap : public FDynamicCubeMap
{
public:
	FDynamicReflectionCubeMap();
	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer) override;
	void PreDraw(float DeltaTime) override;
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info) override;
	void Build(const XMFLOAT3& center) override;

	void BuildDepthStencilDescriptor() override;

	// �жϳ������Ƿ���ڶ�̬����
	bool IsExitDynamicReflectionMesh();

};
