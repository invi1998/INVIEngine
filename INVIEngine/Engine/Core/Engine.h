#pragma once

#include "WinMainCommandParameters.h"
#include "CoreObject/CoreMinimalObject.h"

class CEngine : public CCoreMinimalObject
{
public:
	CEngine();
	virtual ~CEngine() override = default;

	// 引擎预初始化
	virtual int PreInit(
#if defined(_WIN32)
		class FWinMainCommandParameters InParameters
#endif
	) = 0;

	// 引擎初始化
	virtual int Init(
#if defined(_WIN32)
		class FWinMainCommandParameters InParameters
#endif
	) = 0;

	// 引擎初始化完毕
	virtual int PostInit() = 0;

	// 引擎刷新
	virtual void Tick(float DeltaTime) override {};

	// 引擎预退出
	virtual int PreExit() = 0;

	// 引擎退出
	virtual int Exit() = 0;

	// 引擎退出完毕
	virtual int PostExit() = 0;

};

