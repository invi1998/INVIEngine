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

public:
	void SetBaseColor(const XMFLOAT4& color) { BaseColor = color; }
	void SetMaterialType(const EMaterialType type) { MaterialType = type; }
	void SetRoughness(const float roughness) { Roughness = roughness; }
	void SetMaterialDisplayStatus(EMaterialDisplayStatusType type) { MaterialDisplayType = type; };
	void SetTransformation(const XMFLOAT4X4& transform) { MaterialTransformation = transform; }

private:
	XMFLOAT4 BaseColor;
	float Roughness;		// ²ÄÖÊ´Ö²Ú¶È
	EMaterialType MaterialType = EMaterialType::Lambert;
	EMaterialDisplayStatusType MaterialDisplayType = EMaterialDisplayStatusType::TriangleDisplay;
	XMFLOAT4X4 MaterialTransformation;
};

