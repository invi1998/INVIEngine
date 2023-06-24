#pragma once

#include "../../Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
	virtual int PreInit(FWinMainCommandParameters InParameters) override;
	virtual int Init() override;
	virtual int PostInit() override;

	virtual void Tick() override;

	virtual int PreExit() override;
	virtual int Exit() override;
	virtual int PostExit() override;

};
