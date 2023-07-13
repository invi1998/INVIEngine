#pragma once

#include "Core/Engine.h"
#include "Platform/Windows/WindowsEngine.h"

class IRenderingInterface // : public IGuidInterface
{
	friend class CDirectXRenderingEngine;

public:
	IRenderingInterface();
	virtual ~IRenderingInterface();	// override;

	virtual void Init();

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

protected:
	// 构造默认缓冲区
	ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer, const void* InData, UINT64 InDataSize);

protected:

	ComPtr<ID3D12Device> GetD3dDevice();
	ComPtr<ID3D12GraphicsCommandList> GetD3dGraphicsCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();

	[[nodiscard]] CWindowsEngine* GetEngine() const;

//private:
//	static std::vector<IRenderingInterface*> RenderingInterface;
};

