#pragma once

#include "GuidInterface.h"
#include "Core/Construction/ObjectConstruction.h"

class CCoreMinimalObject : public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject() override;

	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	bool IsTick() const { return bTick; }

	void SetOwner(CCoreMinimalObject* InOwner) { Owner = InOwner; }		// ����������

	// ��ȡ������
	FORCEINLINE CCoreMinimalObject* GetOwner() const { return Owner; }

protected:
	bool bTick;

	// owner ������, һ����һ�����, ����Ҳ�п�����һ������
	CCoreMinimalObject* Owner;
};

extern std::vector<CCoreMinimalObject*> GObjects;		// ȫ�ֶ���


