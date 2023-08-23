#pragma once

#include "MeshType.h"
#include "Rendering/Core/Rendering.h"

class CTransformationComponent;

class CMesh : public CCoreMinimalObject, public IRenderingInterface
{

	CVARIABLE()
		CTransformationComponent* TransformationComponent;

public:
	CMesh();
	~CMesh() override;

	virtual void Init() override;

	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
	virtual void PostDraw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

public:
	void SetPosition(const XMFLOAT3& InNewPosition);
	void SetRotation(const fvector_3d& InRotation);
	void SetScale(const fvector_3d& InNewScale);

public:
	XMFLOAT3& GetPosition();
	XMFLOAT3 GetForwardVector();
	XMFLOAT3 GetRightVector();
	XMFLOAT3 GetUpVector();
	
};

