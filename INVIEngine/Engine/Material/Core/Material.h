#pragma once

#include "EngineMinimal.h"
#include "Core/CoreObject/CoreMinimalObject.h"

class CMaterial : public CCoreMinimalObject
{
public:
	CMaterial();
	~CMaterial() override;

	void BeginInit() override;
	void Tick(float DeltaTime) override;

	FORCEINLINE XMFLOAT4 GetBaseColor() const { return BaseColor; }
	void SetBaseColor(const XMFLOAT4& color) { BaseColor = color; }

private:
	XMFLOAT4 BaseColor;
};

