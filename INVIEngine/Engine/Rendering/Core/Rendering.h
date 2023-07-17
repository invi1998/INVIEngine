#pragma once

#include "Core/Engine.h"
#include "Platform/Windows/WindowsEngine.h"

class IRenderingInterface // : public IGuidInterface
{
	friend class CDirectXRenderingEngine;
	// friend class IDirectXDeviceInterface;

public:
	IRenderingInterface();
	virtual ~IRenderingInterface();	// override;

	virtual void Init();

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

//private:
//	static std::vector<IRenderingInterface*> RenderingInterface;
};

