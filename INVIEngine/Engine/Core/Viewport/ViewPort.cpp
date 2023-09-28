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

	// ͶӰ���󣨻�����Ұ�����ĵ�����͸��ͶӰ����
	XMMATRIX Project = XMMatrixPerspectiveFovLH(
		0.25f * XM_PI,			// -`FovAngleY`����Ұ�Ĵ�ֱ�Ƕȣ���λ�ǻ��ȡ��Ի���Ϊ��λ�����϶��µ��ӳ���
		AspectRatio,			// - `AspectRatio`����߱ȣ����ӿڿ�ȳ����ӿڸ߶ȡ�
		0.1f,					// - `NearZ`�����ü�����롣
		1000.f					// - `FarZ`��Զ�ü�����롣
	);

	XMStoreFloat4x4(&ProjectionMatrix, Project);
}
