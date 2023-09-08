#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: BaseColor(XMFLOAT4(Colors::Gray)), Roughness(0.2f), MaterialProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
}
