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
}
