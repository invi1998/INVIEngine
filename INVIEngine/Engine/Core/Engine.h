#pragma once

#include "WinMainCommandParameters.h"
#include "CoreObject/CoreMinimalObject.h"

class CEngine : public CCoreMinimalObject
{
public:
	CEngine();
	virtual ~CEngine() override = default;

	// ����Ԥ��ʼ��
	virtual int PreInit(
#if defined(_WIN32)
		class FWinMainCommandParameters InParameters
#endif
	) = 0;

	// �����ʼ��
	virtual int Init(
#if defined(_WIN32)
		class FWinMainCommandParameters InParameters
#endif
	) = 0;

	// �����ʼ�����
	virtual int PostInit() = 0;

	// ����ˢ��
	virtual void Tick(float DeltaTime) override {};

	// ����Ԥ�˳�
	virtual int PreExit() = 0;

	// �����˳�
	virtual int Exit() = 0;

	// �����˳����
	virtual int PostExit() = 0;

};

