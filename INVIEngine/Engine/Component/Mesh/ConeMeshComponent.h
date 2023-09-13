#pragma once

#include "EngineMinimal.h"
#include "ShellMeshComponent.h"

class CConeMeshComponent : public CShellMeshComponent
{
public:
	CConeMeshComponent();

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InBottomRadius,			// Բ���ײ��뾶
		float InHeight,					// �߶�
		uint32_t InAxialSubdivision,	// ����ϸ��
		uint32_t InHeightSubdivision	// �߶�ϸ��
	);
};

