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

	// 是否是动态反射组件
	bool IsDynamicReflection() const;

	// 是否开启投射阴影 (也就是该组件的阴影是否会投射到地面或者其他物体上，否则的话就是一些自己组件的背光阴影)
	bool IsCastShadow() const;

	void SetCastShadow(bool bCastShadow) { bCastShadow = bCastShadow; }

	void SetPickUp(bool pickUp) { bPickUp = pickUp; }

	bool IsPickUp() const { return bPickUp; }
	
public:

	EMeshRenderLayerType MeshRenderLayerType;

protected:
	bool bCastShadow = true;		// 是否开启投射阴影
	bool bPickUp = true;			// 是否可以被拾取（鼠标点击，射线检测）
};

