#pragma once

#include "Mesh/Core/Mesh.h"

class GBoxMesh : public GMesh
{
	typedef GMesh Super;

public:
	GBoxMesh();
	~GBoxMesh() override;

	virtual void Init() override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData) override;

	virtual void Draw(float DeltaTime) override;

	void CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth);
};

