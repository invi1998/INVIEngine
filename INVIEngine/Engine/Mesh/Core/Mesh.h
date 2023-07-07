#pragma once

#include "MeshType.h"
#include "Core/Engine.h"
#include "Rendering/Core/Rendering.h"
#include "Shader/Core/Shader.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;

	static XMFLOAT4X4 IdentityMatrix4X4();	// µ•ŒªªØ4x4æÿ’Û
};

class CMesh : public CCoreMinimalObject, public IRenderingInterface
{
public:
	CMesh();
	~CMesh() override;

	virtual void Init() override;

	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
	virtual void PostDraw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRendingData* InRenderingData);
	

};

