#pragma once

#include "Interface/DirectXDeviceInterface.h"


struct FDirectXDescriptorHeap : public IDirectXDeviceInterface_Struct
{
public:
	void Build(UINT InNumDescriptor);

	ID3D12DescriptorHeap* GetHeap() const { return CBVHeap.Get(); }
	void PreDraw(float delta_time);

protected:
	ComPtr<ID3D12DescriptorHeap> CBVHeap;			// CPU ���������������ָ��Ҫ�����ĳ�����������ͼ���ڵ���������λ��
};

