#pragma once

#include "Mesh/Core/Mesh.h"

class CBoxMesh : public CMesh
{
	typedef CMesh Super;

public:
	CBoxMesh();
	~CBoxMesh() override;

	virtual void Init() override;

	virtual void BuildMesh(const FMeshRendingData* InRenderingData) override;

	virtual void Draw(float DeltaTime) override;

	static CBoxMesh* CreateMesh(float InHeight, float InWidth, float InDepth);
};

