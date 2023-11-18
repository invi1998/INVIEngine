#include "EngineMinimal.h"
#include "DynamicCubeMap.h"

#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"


FDynamicCubeMap::FDynamicCubeMap()
{
}

void FDynamicCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState)
{
	GeometryMap = inGeometryMap;
	DirectXPipelineState = inDirectXPipelineState;
}

void FDynamicCubeMap::BuildViewPort(const XMFLOAT3& InCenterPoint)
{
	struct FTempViewportCapture
	{
		XMFLOAT3 TargetPoint[6];
		XMFLOAT3 Up[6];
	};
	// +x, -x, +y, -y, +z, -z
	FTempViewportCapture Capture{};

	// 捕获摄像机的6个面
	Capture.TargetPoint[0] = XMFLOAT3{ InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z};
	Capture.TargetPoint[1] = XMFLOAT3{ InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z};
	Capture.TargetPoint[2] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z};
	Capture.TargetPoint[3] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z};
	Capture.TargetPoint[4] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f};
	Capture.TargetPoint[5] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f };

	
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 0.f, -1.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 0.f, 1.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f};
	Capture.Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f };

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts.push_back(CreateObject<GClientViewPort>(new GClientViewPort()));
		GClientViewPort* viewport = CubeMapViewPorts[CubeMapViewPorts.size() - 1];

		viewport->FaceTarget(InCenterPoint, Capture.TargetPoint[i], Capture.Up[i]);
		viewport->SetFrustum(XM_PIDIV2, 1, 1, 0.1f, 10000.f);
		viewport->BuildViewMatrix();
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
}
