#include "ViewportTransformation.h"

FViewportTransformation::FViewportTransformation()
	: CameraPosition(0.f,0.f,0.f,0.f)
	, ViewProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
}
