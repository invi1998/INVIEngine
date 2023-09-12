#pragma once

#include "EngineMinimal.h"

#include "MeshType.h"
#include "Actor/Core/ActorObject.h"
#include "Component/Mesh/ShellMeshComponent.h"
#include "Rendering/Core/Rendering.h"

class CShellMeshComponent;
class CMaterial;

class GMesh : public GActorObject, public IRenderingInterface
{
	CVARIABLE()
		CTransformationComponent* TransformationComponent;

	CVARIABLE()
		CShellMeshComponent* ShellMeshComponent;

public:
	GMesh();
	~GMesh() override;

	virtual void Init() override;

	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
	virtual void PostDraw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

public:
	UINT GetMaterialNum() const;

	std::vector<CMaterial*>* GetMaterial();
};

