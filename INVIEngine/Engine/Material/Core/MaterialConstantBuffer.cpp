#include "EngineMinimal.h"

#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: MaterialType(0), BaseColor(XMFLOAT4(Colors::Gray)), Roughness(0.2f), BaseColorIndex(-1), NormalIndex(-1),
	  Transformation(EngineMath::IdentityMatrix4x4())
{
}
