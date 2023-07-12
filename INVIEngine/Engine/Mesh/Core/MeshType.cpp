#include "MeshType.h"

FVertex::FVertex(const DirectX::XMFLOAT3& InPos, const DirectX::XMFLOAT4& InColor)
	: Position(InPos), Color(InColor)
{
}

FMeshRenderingData::FMeshRenderingData(const std::vector<uint16_t>& Index, const std::initializer_list<FVertex>& Vertex)
		: IndexData(Index), VertexData(Vertex)
{
}
