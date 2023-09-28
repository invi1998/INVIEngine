#pragma once

#include "ShellMeshComponent.h"

class CCylinderMeshComponent : public CShellMeshComponent
{
public:
	CCylinderMeshComponent();

	void CreateMesh(
		FMeshRenderingData& MeshData,
		float InTopRadius,				// Բ�������뾶
		float InBottomRadius,			// Բ���ײ��뾶
		float InHeight,					// �߶�
		uint32_t InAxialSubdivision,	// ����ϸ��
		uint32_t InHeightSubdivision	// �߶�ϸ��
	);
};

