#include "EngineMinimal.h"

#include "ViewportTransformation.h"

FViewportTransformation::FViewportTransformation()
	: CameraPosition(0.f,0.f,0.f,1.f)
	, ViewProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
}
