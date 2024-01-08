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

	// �Ƿ��Ƕ�̬�������
	bool IsDynamicReflection() const;

	// �Ƿ���Ͷ����Ӱ (Ҳ���Ǹ��������Ӱ�Ƿ��Ͷ�䵽����������������ϣ�����Ļ�����һЩ�Լ�����ı�����Ӱ)
	bool IsCastShadow() const;

	void SetCastShadow(bool bCastShadow) { bCastShadow = bCastShadow; }

	void SetPickUp(bool pickUp) { bPickUp = pickUp; }

	bool IsPickUp() const { return bPickUp; }
	
public:

	EMeshRenderLayerType MeshRenderLayerType;

protected:
	bool bCastShadow = true;		// �Ƿ���Ͷ����Ӱ
	bool bPickUp = true;			// �Ƿ���Ա�ʰȡ������������߼�⣩
};

