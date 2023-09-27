#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: MaterialType(0), BaseColor(XMFLOAT4(Colors::Gray)), Roughness(0.2f), Reserved1(0), Reserved2(0),
	  Transformation(EngineMath::IdentityMatrix4x4())
{
}
