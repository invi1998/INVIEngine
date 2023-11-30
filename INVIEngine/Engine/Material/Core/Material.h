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
	FORCEINLINE XMFLOAT3 GetSpecularColor() const { return SpecularColor; }
	FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
	FORCEINLINE EMaterialDisplayStatusType GetMaterialDisplayType() const { return MaterialDisplayType; }
	FORCEINLINE float GetRoughness()const { return Roughness; }
	FORCEINLINE XMFLOAT4X4 GetTransformation() const { return MaterialTransformation; }
	FORCEINLINE bool IsDirty() const { return bDirty; }
	FORCEINLINE const std::string& GetBaseColorIndexKey() const { return BaseColorIndexKey; }
	FORCEINLINE const std::string& GetNormalIndexKey() const { return NormalIndexKey; }
	FORCEINLINE const std::string& GetSpecularIndexKey() const { return SpecularIndexKey; }
	FORCEINLINE int GetMaterialID() const { return MaterialID; }
	FORCEINLINE XMFLOAT3 GetFresnelF0() const { return FresnelF0; }
	FORCEINLINE float GetTransparency() const { return Transparency; }
	FORCEINLINE bool IsDynamicReflection() const { return bDynamicReflection && (MaterialType == EMaterialType::BlinnPhong || MaterialType == EMaterialType::PBR || MaterialType == EMaterialType::Phong || MaterialType == EMaterialType::Back); }
	FORCEINLINE float GetRefractiveValue() const { return RefractiveValue; }

public:
	void SetBaseColor(const XMFLOAT4& color);
	void SetMaterialType(const EMaterialType type);
	void SetRoughness(const float roughness);
	void SetMaterialDisplayStatus(EMaterialDisplayStatusType type);
	void SetTransformation(const XMFLOAT4X4& transform);
	void SetBaseColorIndexKey(const std::string& key);
	void SetNormalIndexKey(const std::string& key);
	void SetSpecular(const std::string& key);
	void SetSpecular(const XMFLOAT3& color);
	void SetDirty(bool dirty);
	void SetMaterialID(const int id);
	void SetFresnelF0(const XMFLOAT3& f0);
	void SetTransparency(float x);
	void SetDynamicReflection(bool bValue);
	void SetRefractiveValue(float x);

private:
	// �������־�����ڱ�־��ǰ�����Ƿ񱻶�̬�޸Ĺ����������̬�޸Ĺ�����ֵΪtrue��Ϊtrue����ô���ǲ������ύ�ò���
	bool bDirty;

	int MaterialID;		// ����id

	XMFLOAT4 BaseColor;
	XMFLOAT3 SpecularColor;		// �߹���ɫ
	float Roughness;		// ���ʴֲڶ�
	EMaterialType MaterialType = EMaterialType::Lambert;
	EMaterialDisplayStatusType MaterialDisplayType = EMaterialDisplayStatusType::TriangleDisplay;
	XMFLOAT4X4 MaterialTransformation;

	XMFLOAT3 FresnelF0;				// �����F0

	std::string BaseColorIndexKey;	// ������ͼ
	std::string NormalIndexKey;		// ������ͼ
	std::string SpecularIndexKey;	// �߹���ͼ

	float Transparency;				// ͸����

	bool bDynamicReflection = false;		// �Ƿ��Ƕ�̬�������

	float RefractiveValue;			// ������
};

