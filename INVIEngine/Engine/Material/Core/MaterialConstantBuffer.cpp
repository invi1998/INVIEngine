#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: BaseColor(0.45f, 0.45f, 0.45f, 1.f)
	, Roughness(0.2f)
	, World(EngineMath::IdentityMatrix4x4())
{
}
