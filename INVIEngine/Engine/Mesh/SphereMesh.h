#pragma once
#include "Core/Mesh.h"

class GSphereMesh : public GMesh
{
public:
	GSphereMesh();
	void Init() override;
	void Draw(float DeltaTime) override;

	void CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision, bool bReverse = false);

};

