#pragma once
#include "Rendering/Core/DirectX12/RenderingPipeline/DynamicMap/Core/DynamicMap.h"


class GClientViewPort;

class FDynamicShadowMap:public FDynamicMap
{
public:
	FDynamicShadowMap();
	~FDynamicShadowMap() override;

	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer) override;
	void PreDraw(float DeltaTime) override;
	void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info) override;
	void Build(const XMFLOAT3& center) override;
	void Draw(float deltaTime) override;

public:
	void SetViewportPosition(const XMFLOAT3& position);
	void SetViewportRotation(const XMFLOAT3& rotation);
	void BuildViewPort(const XMFLOAT3& position);

	XMFLOAT4X4 GetViewMatrix();

	XMFLOAT4X4 GetProjectionMatrix();

	void BuildViewMatrix();

	void BuildOrthographicOffCenterProjectionMatrixLH(float radius, const XMFLOAT3& centerPos);
	void BuildPerspectiveProjectionMatrixLH(float radius, const XMFLOAT3& centerPos);

	// 构建投影矩阵
	void BuildProjectionMatrix();

	// 构建正交投影矩阵（平行光）目标位置 方向 半径
	void BuildOrthoProjectionMatrix(const XMFLOAT3& targetPosition, const XMFLOAT3& direction, float radius);

	// 构建透视投影矩阵（聚光灯）光源位置，光源方向，半径
	void BuildPerspectiveProjectionMatrix(const XMFLOAT3& position, const XMFLOAT3& direction, float radius);

	// 构建深度模板描述
	void BuildDepthStencilViewDesc();
	void BuildShadowMapRenderTargetDescriptor();

	void DrawShadowMapTexture(float DeltaTime);

protected:
	void BuildRenderTargetSRV();

protected:
	GClientViewPort* ShadowViewPort = nullptr;		// ShadowMap视口

};

