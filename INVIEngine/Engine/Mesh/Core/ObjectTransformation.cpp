#include "EngineMinimal.h"

#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
	: World(EngineMath::IdentityMatrix4x4()), TextureTransformation(EngineMath::IdentityMatrix4x4()), NormalTransformation(EngineMath::IdentityMatrix4x4()), MaterialID(0),
	  rr1(0), rr2(0), rr3(0)
{
}

