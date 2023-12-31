#include "EngineMinimal.h"

#include "MaterialConstantBuffer.h"

FMaterialConstantBuffer::FMaterialConstantBuffer()
	: MaterialType(0),
	  BaseColor(XMFLOAT4(Colors::Gray)),
	  SpecularColor(),
	  RefractiveValue(0), FresnelF0({0.04f, 0.04f, 0.04f}), Transparency(0),
	  Roughness(0.2f),
	  BaseColorIndex(-1),
	  NormalIndex(-1),
	  SpecularIndex(0),
	  XX1(0), XX2(0), XX3(0),
	  Transformation(EngineMath::IdentityMatrix4x4()),
	Metallicity(0.2f, 0.2f, 0.2f)
{
}
