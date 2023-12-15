#include "EngineMinimal.h"

#include "PlaneMeshComponent.h"

#include "Mesh/Core/MeshType.h"

CPlaneMeshComponent::CPlaneMeshComponent()
{
}

void CPlaneMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
	auto SubdivideValue = [&](float InValue, uint32_t InSubdivideValue)->float
		{
			if (InSubdivideValue <= 1)
			{
				return InValue;
			}

			return InValue / ((float)InSubdivideValue - 1);
		};

	float CHeight = 0.5f * InHeight;
	float CWidth = 0.5f * InWidth;

	float HeightSubdivideValue = SubdivideValue(InHeight, InHeightSubdivide);
	float WidthSubdivideValue = SubdivideValue(InWidth, InWidthSubdivide);

	float HorizontalAverageSubdivision = 1.f / ((float)InWidthSubdivide - 1.f);
	float VerticalAverageSubdivision = 1.f / ((float)InHeightSubdivide - 1.f);

	//绘制点的位置
	for (uint32_t i = 0; i < InHeightSubdivide; ++i)
	{
		float Z = CHeight - i * HeightSubdivideValue;
		for (uint32_t j = 0; j < InWidthSubdivide; ++j)
		{
			float X = CWidth - j * WidthSubdivideValue;
			MeshData.VertexData.push_back(FVertex(
				XMFLOAT3(
					X,//x
					0.f,//y
					Z), //z
				XMFLOAT4(Colors::Gray),
				XMFLOAT3(0.f, 1.f, 0.f),//法线
				XMFLOAT2(VerticalAverageSubdivision * i, HorizontalAverageSubdivision * j)));//UV自动展开
		}
	}

	//绘制index
	for (uint32_t i = 0; i < InHeightSubdivide - 1; ++i)
	{
		for (uint32_t j = 0; j < InWidthSubdivide - 1; ++j)
		{
			////我们绘制的是四边形
			////三角形1
			//MeshData.IndexData.push_back( i * InWidthSubdivide + j);
			//MeshData.IndexData.push_back( i * InWidthSubdivide + j + 1);
			//MeshData.IndexData.push_back( (i + 1) * InWidthSubdivide + j);

			////三角形2
			//MeshData.IndexData.push_back( (i + 1) * InWidthSubdivide + j);
			//MeshData.IndexData.push_back( i * InWidthSubdivide + j + 1);
			//MeshData.IndexData.push_back( (i + 1) * InWidthSubdivide + j + 1);

			//我们绘制的是四边形
			//三角形1
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j);

			//三角形2
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
			MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
		}
	}
}

void CPlaneMeshComponent::BuildKey(size_t& meshKey, float InHeight, float InWidth, uint32_t InHeightSubdivide,
	uint32_t InWidthSubdivide)
{
	constexpr std::hash<float> floatHash;

	meshKey = 6;
	meshKey += floatHash(InHeight);
	meshKey += floatHash(InWidth);

	meshKey += std::hash<int>::_Do_hash(InHeightSubdivide);

}
