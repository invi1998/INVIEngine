#pragma once

#include "Mesh/Core/Mesh.h"

class FBoxMesh : public FMesh
{
	typedef FMesh Super;

public:
	FBoxMesh();
	~FBoxMesh() override;

	virtual void Init() override;

	virtual void BuildMesh(const FMeshRendingData* InRenderingData) override;

	virtual void Draw(float DeltaTime) override;

	static FBoxMesh* CreateMesh();
};

