#include "EngineMinimal.h"

#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: MaterialType(0), BaseColor(XMFLOAT4(Colors::Gray)), SpecularColor(), XX6(0), Roughness(0.2f), BaseColorIndex(-1),
	  NormalIndex(-1), SpecularIndex(0), XX1(0), XX2(0), XX3(0),
	  Transformation(EngineMath::IdentityMatrix4x4())
{
}
