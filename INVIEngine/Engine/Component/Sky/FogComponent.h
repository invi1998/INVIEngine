#pragma once
#include "Component/Core/Component.h"

class CFogComponent : public CComponent
{
public:
	CFogComponent();

public:
	void SetFogColor(const XMFLOAT4& color);
	void SetFogStart(float start);
	void SetFogRange(float range);
	void SetFogHeight(float height);
	void SetFogTransparentCoefficient(float value);
	void SetDirty(bool dirty);

	FORCEINLINE XMFLOAT4 GetFogColor() const { return FogColor; }
	FORCEINLINE float GetFogStart() const { return FogStart; }
	FORCEINLINE float GetFogRange() const { return FogRange; }
	FORCEINLINE float GetFogHeight() const { return FogHeight; }
	FORCEINLINE float GetFogTransparentCoefficient() const { return FogTransparentCoefficient; }
	FORCEINLINE bool IsDirty() const { return bDirty; }

protected:
	XMFLOAT4 FogColor;	// 雾的颜色

	float FogStart;		// 雾生成的起始点
	float FogRange;		// 雾的范围

	float FogHeight;	// 雾的高度

	float FogTransparentCoefficient;		// 雾的透明系数

	bool bDirty;


};

