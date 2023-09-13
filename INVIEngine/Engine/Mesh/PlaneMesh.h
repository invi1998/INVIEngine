#pragma once
#include "EngineMinimal.h"
#include "Core/Mesh.h"

class GPlaneMesh : public GMesh
{
public:
	void Init() override;
	void Draw(float DeltaTime) override;

	void CreateMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};

