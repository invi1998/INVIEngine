#include "EngineMinimal.h"
#include "SSAOConstant.h"

FSSAOConstant::FSSAOConstant()
{
	XMStoreFloat4x4(&InversiveProjectionMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&ProjectionMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&TextureProjectionMatrix, XMMatrixIdentity());
}

FSSAOBlurParam::FSSAOBlurParam()
{
	BlurWeight[0] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	BlurWeight[1] = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	BlurWeight[2] = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
}
