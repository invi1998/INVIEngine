#pragma once

#include "Mesh/Core/Mesh.h"

// Ô²Öù¡¢Ô²Ì¨
class GCylinderMesh : public GMesh
{
public:
	void Init() override;
	void Draw(float DeltaTime) override;

	void CreateMesh(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);
};

