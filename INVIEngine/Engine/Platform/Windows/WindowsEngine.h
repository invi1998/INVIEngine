#pragma once

#include "Core/Engine.h"

class CWorld;
class CDirectXRenderingEngine;

class CWindowsEngine : public CEngine
{
	friend class IRenderingInterface;

	friend class IDirectXDeviceInterface;

public:
	CWindowsEngine();
	~CWindowsEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters) override;
	virtual int Init(FWinMainCommandParameters InParameters) override;
	virtual int PostInit() override;

	virtual void Tick(float DeltaTime) override;

	virtual int PreExit() override;
	virtual int Exit() override;
	virtual int PostExit() override;

	// ³õÊ¼»¯´°¿Ú
	bool InitWindows(FWinMainCommandParameters InParameters);

	CDirectXRenderingEngine* GetRenderingEngine() { return RenderingEngine; }

protected:
	HWND MainWindowsHandle;

	CDirectXRenderingEngine* RenderingEngine;

	CWorld* World;
};
