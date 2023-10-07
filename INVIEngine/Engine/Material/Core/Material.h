#pragma once

#include "MaterialType.h"
#include "Core/CoreObject/CoreMinimalObject.h"

class CMaterial : public CCoreMinimalObject
{
public:
	CMaterial();
	~CMaterial() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;

public:
	FORCEINLINE XMFLOAT4 GetBaseColor() const { return BaseColor; }
	FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
	FORCEINLINE EMaterialDisplayStatusType GetMaterialDisplayType() const { return MaterialDisplayType; }
	FORCEINLINE float GetRoughness()const { return Roughness; }
	FORCEINLINE XMFLOAT4X4 GetTransformation() const { return MaterialTransformation; }
	FORCEINLINE bool IsDirty() const { return bDirty; }
	FORCEINLINE const std::string& GetBaseColorIndexKey() const { return BaseColorIndexKey; }
	FORCEINLINE int GetMaterialID() const { return MaterialID; }

public:
	void SetBaseColor(const XMFLOAT4& color);
	void SetMaterialType(const EMaterialType type);
	void SetRoughness(const float roughness);
	void SetMaterialDisplayStatus(EMaterialDisplayStatusType type);
	void SetTransformation(const XMFLOAT4X4& transform);
	void SetBaseColorIndexKey(const std::string& key);
	void SetDirty(bool dirty);
	void SetMaterialID(const int id);

private:
	// �������־�����ڱ�־��ǰ�����Ƿ񱻶�̬�޸Ĺ����������̬�޸Ĺ�����ֵΪtrue��Ϊtrue����ô���ǲ������ύ�ò���
	bool bDirty;

	int MaterialID;		// ����id

	XMFLOAT4 BaseColor;
	float Roughness;		// ���ʴֲڶ�
	EMaterialType MaterialType = EMaterialType::Lambert;
	EMaterialDisplayStatusType MaterialDisplayType = EMaterialDisplayStatusType::TriangleDisplay;
	XMFLOAT4X4 MaterialTransformation;

	std::string BaseColorIndexKey;

};

