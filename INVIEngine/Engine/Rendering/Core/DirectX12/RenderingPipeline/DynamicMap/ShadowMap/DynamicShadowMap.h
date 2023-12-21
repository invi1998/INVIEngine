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

	// ����ͶӰ����
	void BuildProjectionMatrix();

	// ��������ͶӰ����ƽ�й⣩Ŀ��λ�� ���� �뾶
	void BuildOrthoProjectionMatrix(const XMFLOAT3& targetPosition, const XMFLOAT3& direction, float radius);

	// ����͸��ͶӰ���󣨾۹�ƣ���Դλ�ã���Դ���򣬰뾶
	void BuildPerspectiveProjectionMatrix(const XMFLOAT3& position, const XMFLOAT3& direction, float radius);

	// �������ģ������
	void BuildDepthStencilViewDesc();
	void BuildShadowMapRenderTargetDescriptor();

	void DrawShadowMapTexture(float DeltaTime);

protected:
	void BuildRenderTargetSRV();

protected:
	GClientViewPort* ShadowViewPort = nullptr;		// ShadowMap�ӿ�

};

