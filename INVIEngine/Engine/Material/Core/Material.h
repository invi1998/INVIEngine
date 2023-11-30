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
	// 材质脏标志，用于标志当前材质是否被动态修改过，如果被动态修改过，该值为true，为true，那么我们才重新提交该材质
	bool bDirty;

	int MaterialID;		// 材质id

	XMFLOAT4 BaseColor;
	XMFLOAT3 SpecularColor;		// 高光颜色
	float Roughness;		// 材质粗糙度
	EMaterialType MaterialType = EMaterialType::Lambert;
	EMaterialDisplayStatusType MaterialDisplayType = EMaterialDisplayStatusType::TriangleDisplay;
	XMFLOAT4X4 MaterialTransformation;

	XMFLOAT3 FresnelF0;				// 菲尼尔F0

	std::string BaseColorIndexKey;	// 纹理贴图
	std::string NormalIndexKey;		// 法线贴图
	std::string SpecularIndexKey;	// 高光贴图

	float Transparency;				// 透明度

	bool bDynamicReflection = false;		// 是否是动态反射材质

	float RefractiveValue;			// 折射率
};

