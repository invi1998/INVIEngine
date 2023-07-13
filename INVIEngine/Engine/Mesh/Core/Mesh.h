#pragma once

#include "MeshType.h"
#include "Rendering/Core/Rendering.h"

class CMesh : public CCoreMinimalObject, public IRenderingInterface
{
public:
	CMesh();
	~CMesh() override;

	virtual void Init() override;

	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
	virtual void PostDraw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);
	

};

