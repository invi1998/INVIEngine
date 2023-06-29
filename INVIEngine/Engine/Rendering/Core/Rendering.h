#pragma once

#include "Core/Engine.h"

class IRenderingInterface
{
	friend class FWindowsEngine;

public:
	IRenderingInterface();
	virtual ~IRenderingInterface();

	virtual void Init();
	virtual void Draw(float DeltaTime);

	inline simple_c_guid GetGuid() const { return Guid; }

public:
	bool operator==(const IRenderingInterface& InOther)
	{
		return guid_equal(&Guid, &InOther.Guid);
	}

protected:

	ComPtr<ID3D12Device> GetD3dDevice();
	ComPtr<ID3D12GraphicsCommandList> GetD3dGraphicsCommandList();

private:
	static std::vector<IRenderingInterface*> RenderingInterface;

	simple_c_guid Guid;

};

