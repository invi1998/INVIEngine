#pragma once

#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"

// ������������ͼ

class FRenderingResourcesUpdate;

struct FConstantBufferViews : IDirectXDeviceInterface_Struct
{

public:
	// ��������
	void CreateConstant(UINT ObjectSize, UINT ObjectCount);

	void Update(int Index, const void* InData);

	// ����������
	void BuildConstantBuffer(CD3DX12_CPU_DESCRIPTOR_HANDLE handle, UINT constantBufferNum, UINT handleOffset = 0);

	std::shared_ptr<FRenderingResourcesUpdate> GetConstant() const { return Constants; }

protected:
	std::shared_ptr<FRenderingResourcesUpdate> Constants;		// ������Դ��ָ���ϴ��ѣ�

};
