#include "EngineMinimal.h"

#include "RenderingData.h"

#include "Component/Mesh/Core/MeshComponent.h"

#include "Mesh/Core/MeshType.h"

FRenderingData::FRenderingData()
	: MeshHash(0)
	  , GeometryKey(0), MeshObjectIndex(0), IndexSize(0)
	  , VertexSize(0)
	  , IndexOffsetPosition(0)
	  , VertexOffsetPosition(0)
	  , VertexTypeSize(sizeof(FVertex))
	  , IndexTypeSize(sizeof(uint16_t))
	  , IndexFormat(DXGI_FORMAT_R16_UINT)
	  , WorldMatrix(EngineMath::IdentityMatrix4x4())
	  , TextureTransformationMatrix(EngineMath::IdentityMatrix4x4())
	  , Mesh(nullptr), MeshRenderingData(nullptr)
{
}

FRenderingData::~FRenderingData()
{
}
