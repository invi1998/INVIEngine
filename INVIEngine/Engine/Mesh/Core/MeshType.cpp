#include "MeshType.h"

FVertex::FVertex(const DirectX::XMFLOAT3& InPos, const DirectX::XMFLOAT4& InColor)
	: Position(InPos), Color(InColor), Normal(0.f, 1.f, 0.f), TexCoord(0.f, 0.f)
{
}

FVertex::FVertex(const DirectX::XMFLOAT3& InPos, const DirectX::XMFLOAT4& InColor, const DirectX::XMFLOAT3& InNormal)
	:Position(InPos), Color(InColor), Normal(InNormal), TexCoord(0.f, 0.f)
{
}

FMeshRenderingData::FMeshRenderingData(const std::vector<uint16_t>& Index, const std::initializer_list<FVertex>& Vertex)
		: IndexData(Index), VertexData(Vertex)
{
}
