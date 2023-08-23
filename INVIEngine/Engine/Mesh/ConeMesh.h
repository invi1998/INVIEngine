#pragma once
#include "EngineMinimal.h"
#include "Core/Mesh.h"

// Բ׶
class GConeMesh : public GMesh
{
public:
	void BeginInit() override;
	void Tick(float DeltaTime) override;
	~GConeMesh() override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InBottomRadius,			// Բ���ײ��뾶
		float InHeight,					// �߶�
		uint32_t InAxialSubdivision,	// ����ϸ��
		uint32_t InHeightSubdivision	// �߶�ϸ��
	);
};

