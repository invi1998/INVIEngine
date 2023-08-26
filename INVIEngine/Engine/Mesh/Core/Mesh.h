#pragma once

#include "MeshType.h"
#include "Actor/Core/ActorObject.h"
#include "Rendering/Core/Rendering.h"

class CMaterial;

class GMesh : public GActorObject, public IRenderingInterface
{
	CVARIABLE()
		std::vector<CMaterial*> Materials;

public:
	GMesh();
	~GMesh() override;

	virtual void Init() override;

	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
	virtual void PostDraw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

public:
	UINT GetMaterialNumber() const;
	
};

