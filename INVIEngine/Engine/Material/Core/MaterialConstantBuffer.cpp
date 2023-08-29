#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: BaseColor(0.18f, 0.15f, 0.13f, 1.f)
	, World(EngineMath::IdentityMatrix4x4())
{
}
