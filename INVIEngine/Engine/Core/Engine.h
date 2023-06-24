#pragma once

#if defined(_WIN32)

#include "WinMainCommandParameters.h"

#endif


class FEngine
{
public:
	// 引擎预初始化
	virtual int PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters InParameters
#endif
	) = 0;

	// 引擎初始化
	virtual int Init() = 0;

	// 引擎初始化完毕
	virtual int PostInit() = 0;

	// 引擎刷新
	virtual void Tick() = 0;

	// 引擎预退出
	virtual int PreExit() = 0;

	// 引擎退出
	virtual int Exit() = 0;

	// 引擎退出完毕
	virtual int PostExit() = 0;

};

