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
	XMFLOAT4 FogColor;	// �����ɫ

	float FogStart;		// �����ɵ���ʼ��
	float FogRange;		// ��ķ�Χ

	bool bDirty;


};

