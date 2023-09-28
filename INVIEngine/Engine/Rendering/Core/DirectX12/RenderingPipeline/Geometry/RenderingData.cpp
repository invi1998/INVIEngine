#include "EngineMinimal.h"

#include "RenderingData.h"

#include "Component/Mesh/Core/MeshComponent.h"

#include "Mesh/Core/MeshType.h"

FRenderingData::FRenderingData()
	: IndexSize(0)
	, VertexSize(0)
	, IndexOffsetPosition(0)
	, VertexOffsetPosition(0)
	, VertexTypeSize(sizeof(FVertex))
	, IndexTypeSize(sizeof(uint16_t))
	, IndexFormat(DXGI_FORMAT_R16_UINT)
	, MaterialTransformationMatrix(EngineMath::IdentityMatrix4x4())
	, TextureTransformationMatrix(EngineMath::IdentityMatrix4x4())
	, Mesh(nullptr)
{
}

FRenderingData::~FRenderingData()
{
}
