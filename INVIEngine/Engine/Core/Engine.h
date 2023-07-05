#pragma once

#if defined(_WIN32)

#include "WinMainCommandParameters.h"
#include "Core/CoreObject/CoreMinimalObject.h"

#endif


class CEngine : public CCoreMinimalObject
{
public:
	CEngine() = default;
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
	virtual void Tick(float DeltaTime) = 0;

	// ����Ԥ�˳�
	virtual int PreExit() = 0;

	// �����˳�
	virtual int Exit() = 0;

	// �����˳����
	virtual int PostExit() = 0;

};

