#pragma once

#include "MeshType.h"
#include "Actor/Core/ActorObject.h"
#include "Component/Mesh/Core/ShellMeshComponent.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/Rendering.h"

class CMaterial;

class GMesh : public GActorObject, public IRenderingInterface, public IDirectXDeviceInterface
{
	// using Super = GActorObject;

	CVARIABLE()
		CTransformationComponent* TransformationComponent;

	CVARIABLE()
		CMeshComponent* MeshComponent;

public:
	GMesh();
	~GMesh() override;

	virtual void Init() override;

	virtual void PreDraw(float DeltaTime) override;
	virtual void Draw(float DeltaTime) override;
	virtual void PostDraw(float DeltaTime) override;

public:
	UINT GetMaterialNum() const;

	std::vector<CMaterial*>* GetMaterial();

	virtual CMeshComponent* GetMeshComponent() { return MeshComponent; }

	void SetCastShadow(bool castShadow);

	bool IsCastShadow() const;

	void SetPickUp(bool cond);

	bool IsPickUp() const;

	template<typename T>
	T* GetMeshComponent()
	{
		return dynamic_cast<T*>(MeshComponent);
	}

public:
	virtual void SetPosition(const XMFLOAT3& InNewPosition) override;
	virtual void SetRotation(const XMFLOAT3& InRotation) override;
	virtual void SetScale(const XMFLOAT3& InNewScale) override;
	void SetMeshRenderLayerType(EMeshRenderLayerType type);

protected:
	virtual void SetMeshComponent(CMeshComponent* InMeshComponent);
};

