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
	// 材质脏标志，用于标志当前材质是否被动态修改过，如果被动态修改过，该值为true，为true，那么我们才重新提交该材质
	bool bDirty;

	int MaterialID;		// 材质id

	XMFLOAT4 BaseColor;
	float Roughness;		// 材质粗糙度
	EMaterialType MaterialType = EMaterialType::Lambert;
	EMaterialDisplayStatusType MaterialDisplayType = EMaterialDisplayStatusType::TriangleDisplay;
	XMFLOAT4X4 MaterialTransformation;

	std::string BaseColorIndexKey;

};

