#pragma once

#include "Core/Engine.h"
#include "Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"

class CWindowsEngine : public CEngine
{
	friend class IRenderingInterface;

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


protected:
	HWND MainWindowsHandle;

	CDirectXRenderingEngine* RenderingEngine;
};
