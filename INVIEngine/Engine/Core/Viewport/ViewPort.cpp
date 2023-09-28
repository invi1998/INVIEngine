#include "EngineMinimal.h"

#include "ViewPort.h"

#include "Config/EngineRenderConfig.h"
#include "Mesh/Core/ObjectTransformation.h"

FViewPort::FViewPort()
	: ViewMatrix(EngineMath::IdentityMatrix4x4()),
	ProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
}

void FViewPort::ViewPortInit()
{

	float AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);

	// 投影矩阵（基于视野构建的的左手透视投影矩阵）
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI,			// -`FovAngleY`：视野的垂直角度，单位是弧度。以弧度为单位的自上而下的视场角
		AspectRatio,			// - `AspectRatio`：宽高比，即视口宽度除以视口高度。
		0.1f,					// - `NearZ`：近裁剪面距离。
		1000.f					// - `FarZ`：远裁剪面距离。
	);

	XMStoreFloat4x4(&ProjectionMatrix, Project);
}
