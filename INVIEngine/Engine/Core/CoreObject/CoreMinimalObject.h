#pragma once

#include "GuidInterface.h"
#include "Core/Construction/ObjectConstruction.h"

class CCoreMinimalObject;

class CCoreMinimalObject : public IGuidInterface
{
public:
	CCoreMinimalObject();
	virtual ~CCoreMinimalObject() override;

	virtual void BeginInit();
	virtual void Tick(float DeltaTime);

	bool IsTick() const { return bTick; }

	void SetOwner(CCoreMinimalObject* InOwner) { Owner = InOwner; }		// ����������
	void SetName(std::string InName) { Name = InName; }					// ��������

	// ��ȡ������
	FORCEINLINE CCoreMinimalObject* GetOwner() const { return Owner; }
	FORCEINLINE std::string GetName() const { return Name; }

protected:
	bool bTick;

	// owner ������, һ����һ�����, ����Ҳ�п�����һ������
	CCoreMinimalObject* Owner;
	std::string Name;
};

extern std::vector<CCoreMinimalObject*> GObjects;		// ȫ�ֶ���


