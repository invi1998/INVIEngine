#include "RenderingData.h"

#include "Mesh/Core/MeshType.h"

FRenderingData::FRenderingData()
	: IndexSize(0)
	, VertexSize(0)
	, IndexOffsetPosition(0)
	, VertexOffsetPosition(0)
	, VertexTypeSize(sizeof(FVertex))
	, IndexTypeSize(sizeof(uint16_t))
{
}
