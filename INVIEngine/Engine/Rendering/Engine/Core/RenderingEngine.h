#pragma once

#include "EngineMinimal.h"

#include "Core/Engine.h"

class CRenderingEngine : public CEngine
{
public:
	void SetMainWindowsHandle(HWND InNewMainWindowsHandle);

protected:
	HWND MainWindowsHandle;
};

