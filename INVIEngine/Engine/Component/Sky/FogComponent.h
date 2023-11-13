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
	void SetDirty(bool dirty);

	FORCEINLINE XMFLOAT4 GetFogColor() const { return FogColor; }
	FORCEINLINE float GetFogStart() const { return FogStart; }
	FORCEINLINE float GetFogRange() const { return FogRange; }
	FORCEINLINE bool IsDirty() const { return bDirty; }

protected:
	XMFLOAT4 FogColor;	// 雾的颜色

	float FogStart;		// 雾生成的起始点
	float FogRange;		// 雾的范围

	bool bDirty;


};

