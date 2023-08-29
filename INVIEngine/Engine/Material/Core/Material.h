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

	void SetBaseColor(const XMFLOAT4& color) { BaseColor = color; }
	void SetMaterialType(const EMaterialType type) { MaterialType = type; }

private:
	XMFLOAT4 BaseColor;
	EMaterialType MaterialType = EMaterialType::Lambert;
};

