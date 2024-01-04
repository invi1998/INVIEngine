#include "EngineMinimal.h"

#include "ViewPort.h"

#include "Config/EngineRenderConfig.h"
#include "Mesh/Core/ObjectTransformation.h"

FViewPort::FViewPort()
	: ViewMatrix(EngineMath::IdentityMatrix4x4()),
	ProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
	ResetViewPort(FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
	ResetViewPortRect(FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight);
}

void FViewPort::ViewPortInit()
{

	float AspectRatio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);

	// ͶӰ���󣨻�����Ұ�����ĵ�����͸��ͶӰ����
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI,			// -`FovAngleY`����Ұ�Ĵ�ֱ�Ƕȣ���λ�ǻ��ȡ��Ի���Ϊ��λ�����϶��µ��ӳ���
		AspectRatio,			// - `AspectRatio`����߱ȣ����ӿڿ�ȳ����ӿڸ߶ȡ�
		0.1f,					// - `NearZ`�����ü�����롣
		10000.f					// - `FarZ`��Զ�ü�����롣
	);

	XMStoreFloat4x4(&ProjectionMatrix, Project);
}

// �����ӿ�
void FViewPort::ResetViewPort(UINT width, UINT height)
{
	// �����ӿ�
	
	ViewPortInfo.TopLeftX = 0;
	ViewPortInfo.TopLeftY = 0;
	ViewPortInfo.Width = static_cast<float>(width);
	ViewPortInfo.Height = static_cast<float>(height);
	ViewPortInfo.MinDepth = 0.0f;
	ViewPortInfo.MaxDepth = 1.0f;

	// �����ӿ�
	// CommandList->RSSetViewports(1, &viewPort);
	// CommandList->RSSetScissorRects(1, &scissorRect);
}

void FViewPort::ResetViewPortRect(UINT width, UINT height)
{
	// ���òü�����
	ViewPortRect.left = 0;
	ViewPortRect.top = 0;
	ViewPortRect.right = static_cast<LONG>(width);
	ViewPortRect.bottom = static_cast<LONG>(height);

	// �����ӿ�
	// CommandList->RSSetScissorRects(1, &scissorRect);
}
