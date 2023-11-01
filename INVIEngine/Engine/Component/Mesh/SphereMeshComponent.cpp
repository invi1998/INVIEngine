#include "EngineMinimal.h"

#include "SphereMeshComponent.h"

#include "Mesh/Core/MeshType.h"

CSphereMeshComponent::CSphereMeshComponent()
{
}

void CSphereMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	float Theta = XM_2PI / static_cast<float>(InHeightSubdivision);
	float Beta = XM_PI / static_cast<float>(InAxialSubdivision);

	// 顶部
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0, InRadius, 0), XMFLOAT4(Colors::White), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT2(1.f, 0.5f)));

	for (size_t i = 1; i < InAxialSubdivision; ++i)
	{
		float BetaValue = i * Beta;

		for (size_t j = 0; j <= InHeightSubdivision; ++j)
		{
			float ThetaValue = j * Theta;

			// 球面上该顶点坐标
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					InRadius * sinf(BetaValue) * cosf(ThetaValue),
					InRadius * cosf(BetaValue),
					InRadius * sinf(BetaValue) * sinf(ThetaValue)
				),
				XMFLOAT4(Colors::White)
			));

			int TopIndex = MeshData.VertexData.size() - 1;
			FVertex& InVertex = MeshData.VertexData[TopIndex];
			// 求球面上该点的法线
			XMVECTOR Pos = XMLoadFloat3(&InVertex.Position);
			XMStoreFloat3(&InVertex.Normal, XMVector3Normalize(Pos));

			// U方向的切线
			InVertex.UTangent.x = -InRadius * sinf(BetaValue) * sinf(ThetaValue);
			InVertex.UTangent.y = 0.f;
			InVertex.UTangent.z = InRadius * sinf(BetaValue) * cosf(ThetaValue);

			// uv
			InVertex.TexCoord.x = ThetaValue / XM_2PI;
			InVertex.TexCoord.y = BetaValue / XM_PI;

			// 计算出切线后，对切线值进行归一化存储
			XMVECTOR Tangent = XMLoadFloat3(&InVertex.UTangent);
			XMStoreFloat3(&InVertex.UTangent, XMVector3Normalize(Tangent));
		}
	}

	// 底部
	MeshData.VertexData.push_back(FVertex(XMFLOAT3(0, -InRadius, 0), XMFLOAT4(Colors::White), XMFLOAT3(0.f, -1.f, 0.f), XMFLOAT2(0.f, 0.5f)));


	// 构建顶点索引 (绘制北极）

	for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
	{
		// 因为DX是左手螺旋定则，所以需要逆时针绘制顶点，法线才能朝外
		MeshData.IndexData.push_back(0);
		MeshData.IndexData.push_back(i + 1);
		MeshData.IndexData.push_back(i);
	}

	uint32_t BaseIndex = 1;
	// 绘制中间区域的点
	uint32_t VertexCircleNum = InAxialSubdivision + 1;
	for (uint32_t i = 0; i < InHeightSubdivision - 2; ++i)
	{
		for (uint32_t j = 0; j < InAxialSubdivision; ++j)
		{
			// 在球体的南北极中间绘制的面是四边形(而一个四边形又是由两个三角形组成的
			// 三角形1
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			// 三角形2
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j);
			MeshData.IndexData.push_back(BaseIndex + i * VertexCircleNum + j + 1);
			MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleNum + j + 1);
		}
	}

	// 绘制南极
	uint32_t SouthBaseIndex = MeshData.VertexData.size() - 1;		// 极点索引
	BaseIndex = SouthBaseIndex - VertexCircleNum;			// 极平面绘制点的起始索引（极点减去经线数量）
	for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
	{
		// 因为DX是左手螺旋定则，所以需要逆时针绘制顶点，法线才能朝外
		MeshData.IndexData.push_back(SouthBaseIndex);
		MeshData.IndexData.push_back(BaseIndex + i);
		MeshData.IndexData.push_back(BaseIndex + i + 1);
	}
}

void CSphereMeshComponent::BuildKey(size_t& meshKey, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	constexpr std::hash<float> floatHash;

	meshKey = 8;
	meshKey += floatHash(InRadius);

	meshKey += std::hash<int>::_Do_hash(InAxialSubdivision);
	meshKey += std::hash<int>::_Do_hash(InHeightSubdivision);
}
