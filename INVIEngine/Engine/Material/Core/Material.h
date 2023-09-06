#pragma once

#include "EngineMinimal.h"
#include "MaterialType.h"
#include "Core/CoreObject/CoreMinimalObject.h"

class CMaterial : public CCoreMinimalObject
{
public:
	CMaterial();
	~CMaterial() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;

	FORCEINLINE XMFLOAT4 GetBaseColor() const { return BaseColor; }
	FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
	FORCEINLINE float GetRoughness()const { return Roughness; }

	void SetBaseColor(const XMFLOAT4& color) { BaseColor = color; }
	void SetMaterialType(const EMaterialType type) { MaterialType = type; }
	void SetRoughness(const float roughness) { Roughness = roughness; }

private:
	XMFLOAT4 BaseColor;
	float Roughness;		// ²ÄÖÊ´Ö²Ú¶È
	EMaterialType MaterialType = EMaterialType::Lambert;
};

