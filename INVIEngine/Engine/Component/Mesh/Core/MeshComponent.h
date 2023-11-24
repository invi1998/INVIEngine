#pragma once

#include "MeshComponentType.h"
#include "Component/TransformationComponent.h"
#include "Material/Core/MaterialType.h"

struct FMeshRenderingData;
class CMaterial;

class CMeshComponent : public CTransformationComponent
{
	CVARIABLE()
		std::vector<CMaterial*> Materials{};

public:
	CMeshComponent();
	~CMeshComponent() override;

	UINT GetMaterialNum() const;

	std::vector<CMaterial*>* GetMaterial() { return &Materials; }

	void BeginInit() override;
	void Tick(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);
	virtual void Init();

	[[nodiscard]] EMeshRenderLayerType GetRenderLayerType() const { return MeshRenderLayerType; }

	void SetRenderLayerType(EMeshRenderLayerType type);

	bool IsDynamicReflection() const;


public:

	EMeshRenderLayerType MeshRenderLayerType;
};

