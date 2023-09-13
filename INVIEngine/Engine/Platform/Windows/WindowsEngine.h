#pragma once

#include "Core/Engine.h"
#include "Light/LightManager.h"

class CMeshManager;
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
	CMeshManager* GetMeshManage();
	CWorld* GetWorld();
	CLightManager* GetLightManage();

protected:
	HWND MainWindowsHandle;

	CDirectXRenderingEngine* RenderingEngine;

	CWorld* World;
};
