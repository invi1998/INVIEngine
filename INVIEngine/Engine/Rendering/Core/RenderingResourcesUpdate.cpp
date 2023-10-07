#include "EngineMinimal.h"

#include "RenderingResourcesUpdate.h"

FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{
}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer != nullptr)
	{
		UploadBuffer->Unmap(0, nullptr);
		UploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount, bool bConstBuffer)
{
	// 断言设备驱动是否可用
	assert(InDevice);

	if (bConstBuffer)
	{
		ElementSize = GetConstantsBufferByteSize(InElementSize);;
	}
	else
	{
		ElementSize = InElementSize;
	}

	// 指定上传堆
	CD3DX12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// 指定资源描述
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ElementSize * InElementCount);
	// 创建提交资源
	ANALYSIS_RESULT(InDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&UploadBuffer)
	));

	// 数据映射
	ANALYSIS_RESULT(UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&Data)));
}

void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	// 通过地址偏移，定位到指定索引的元素的首地址，然后将我们需要更新的数据给内存拷贝的方式拷贝到我们的数据中，拷贝长度就是元素大小
	memcpy(&Data[Index * ElementSize], InData, ElementSize);
}

UINT FRenderingResourcesUpdate::GetConstantsBufferByteSize(UINT InTypeSize)
{
	// 这段代码是用来计算常量缓冲区大小的，它的作用是将所需大小向上取整到 256 字节的整数倍。具体来说，这段代码的含义如下：
	// 首先，将需要分配的空间大小加上 255。这是因为需要进行向上取整操作，而 255 是 256 的最大整数倍减一。
	// 然后，将结果与 0xFF00（即 1111 1111 0000 0000）按位与运算，将低 8 位清零，即向下取整到 256 字节的整数倍。这里使用了 ~255 来代表 0xFF00，因为 ~255 表示将 255 取反，得到的二进制码就是 0xFF。
	// 由此可见，这段代码的作用是将需要分配的空间大小向上取整到 256 字节的整数倍。在实际编程中，这种技巧经常被用于计算资源大小，以保证资源对齐到特定的边界，从而提高效率并避免出现错误。
	return (InTypeSize + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantsBufferByteSize()
{
	return GetConstantsBufferByteSize(ElementSize);
}

