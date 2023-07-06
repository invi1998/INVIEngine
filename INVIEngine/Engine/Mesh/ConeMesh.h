#pragma once
#include "EngineMinimal.h"
#include "Core/Mesh.h"

// Բ׶
class CConeMesh : public CMesh
{
public:
	void BeginInit() override;
	void Tick(float DeltaTime) override;
	~CConeMesh() override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRendingData* InRenderingData) override;

	static CConeMesh* CreateMesh(
		float InBottomRadius,			// Բ���ײ��뾶
		float InHeight,					// �߶�
		uint32_t InAxialSubdivision,	// ����ϸ��
		uint32_t InHeightSubdivision	// �߶�ϸ��
	);
};

