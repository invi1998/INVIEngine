#pragma once

#if defined(_WIN32)

#include "WinMainCommandParameters.h"

#endif


class FEngine
{
public:
	// ����Ԥ��ʼ��
	virtual int PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif
	) = 0;

	// �����ʼ��
	virtual int Init() = 0;

	// �����ʼ�����
	virtual int PostInit() = 0;

	// ����ˢ��
	virtual void Tick() = 0;

	// ����Ԥ�˳�
	virtual int PreExit() = 0;

	// �����˳�
	virtual int Exit() = 0;

	// �����˳����
	virtual int PostExit() = 0;

};

