#pragma once

#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"

// 常量缓冲区视图

class FRenderingResourcesUpdate;

struct FConstantBufferViews : IDirectXDeviceInterface_Struct
{

public:
	// 创建常量
	void CreateConstant(UINT ObjectSize, UINT ObjectCount);

	// 构建缓冲区
	void BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE handle, UINT constantBufferNum, UINT handleOffset = 0);

	std::shared_ptr<FRenderingResourcesUpdate> GetConstant() const { return Constants; }

protected:
	std::shared_ptr<FRenderingResourcesUpdate> Constants;		// 常量资源（指向上传堆）

};

