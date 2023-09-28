#include "EngineMinimal.h"

#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
	: World(EngineMath::IdentityMatrix4x4()), TextureTransformation(EngineMath::IdentityMatrix4x4())
{
}

