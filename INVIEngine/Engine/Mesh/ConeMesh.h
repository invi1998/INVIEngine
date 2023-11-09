#pragma once

#include "Core/Mesh.h"

// Բ׶
class GConeMesh : public GMesh
{
public:
	GConeMesh();
	void Init() override;
	void Draw(float DeltaTime) override;

	void CreateMesh(
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};

