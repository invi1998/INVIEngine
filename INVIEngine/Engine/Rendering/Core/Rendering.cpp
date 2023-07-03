#include "Rendering.h"

#include "Platform/Windows/WindowsEngine.h"

std::vector<IRenderingInterface*> IRenderingInterface::RenderingInterface;

IRenderingInterface::IRenderingInterface()
{
	create_guid(&Guid);

	RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
	for (std::vector<IRenderingInterface*>::const_iterator iter = RenderingInterface.begin(); iter != RenderingInterface.end(); ++iter)
	{
		if (*iter == this)
		{
			RenderingInterface.erase(iter);
			break;
		}
	}
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::Draw(float DeltaTime)
{
}

ComPtr<ID3D12Resource> IRenderingInterface::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& OutTempBuffer,
	const void* InData, UINT64 InDataSize)
{
	ComPtr<ID3D12Resource> Buffer;
	// 资源描述
	CD3DX12_RESOURCE_DESC BufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);

	// 创建默认堆
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// 创建提交资源
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&BufferResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(Buffer.GetAddressOf())
	));

	// 临时的上传缓冲区
	CD3DX12_HEAP_PROPERTIES UpdateBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// 创建提交资源
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&UpdateBufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&BufferResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,		// 设置可读，让当前资源在渲染流水线中可读，因为这个临时上传缓冲区我们在流水线中是需要做写入的
		nullptr,
		IID_PPV_ARGS(OutTempBuffer.GetAddressOf())
	));

	// 描述子资源
	D3D12_SUBRESOURCE_DATA SubResourceData = {};
	SubResourceData.pData = InData;		// 资源数据指针，指向传入的那个资源地址
	SubResourceData.RowPitch = InDataSize;		// 资源行间距，宽度，也就是资源的物理大小，他是以字节为大小的
	SubResourceData.SlicePitch = SubResourceData.RowPitch;		// 代表资源的深度间距，也是字节大小

	// 标记资源为复制目标 （因为我们需要把上传的资源复制到GPU，所以复制的目标需要指定标记出来）
	CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		Buffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,		// 资源当前状态
		D3D12_RESOURCE_STATE_COPY_DEST);	// 资源目标状态 指示资源可用作内存复制操作的目标

	// 执行资源状态转换，并同步GPU
	GetD3dGraphicsCommandList()->ResourceBarrier(1, &CopyDestBarrier);

	// 更新子资源相关内容
	// 将数据从 CPU 内存复制到 GPU 内存中的资源。通常情况下，UpdateSubresources 函数是用来更新上传堆（Upload Heap）中的顶点和索引缓冲区以及纹理资源（Texture Resource）。
	// UpdateSubresources<1> 是 Direct3D 12 中的一个函数模板，用于将单个子资源中的数据从 CPU 内存复制到 GPU 内存中的资源。
	// 与 UpdateSubresources 函数不同，UpdateSubresources<1> 可以直接操作单个子资源，而不需要调用 CopyBufferRegion()、CopyTextureRegion() 或者 CopyResource() 等函数。
	UpdateSubresources<1>(
		GetD3dGraphicsCommandList().Get(),		// 指向要执行的命令列表
		Buffer.Get(),							// 目标资源对象指针
		OutTempBuffer.Get(),					// 中间（临时）资源对象指针
		0,		// 中间资源的偏移量（以字节为单位）。
		0,		// 资源中第一个子资源的索引。 有效值范围为 0 到 MaxSubresources。
		1,		// 子资源数量大小，<1>这个特化中，该值固定为1 资源中的子资源数。 有效值范围为 1 到 (MaxSubresources - FirstSubresource) 。
		&SubResourceData		// 资源地址
	);

	// 更新完资源后，对资源进行状态转换，将拷贝状态转换为可读状态
	CD3DX12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		Buffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_GENERIC_READ
	);

	return Buffer;
}

ComPtr<ID3D12Device> IRenderingInterface::GetD3dDevice()
{
	if (FWindowsEngine* InEngine = dynamic_cast<FWindowsEngine*>(Engine))
	{
		return InEngine->D3dDevice;
	}

	return nullptr;
}

ComPtr<ID3D12GraphicsCommandList> IRenderingInterface::GetD3dGraphicsCommandList()
{
	if (FWindowsEngine* InEngine = GetEngine())
	{
		return InEngine->GraphicsCommandList;
	}

	return nullptr;
}

FWindowsEngine* IRenderingInterface::GetEngine() const
{
	return dynamic_cast<FWindowsEngine*>(Engine);
}

/**
 * \brief //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */

FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{
}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
	if (UploadBuffer)
	{
		// 解除上传堆子资源的映射（以便让CPU中的指针失效）将映射到 CPU 内存空间的缓冲区对象解除映射，并使其可以被 GPU 访问。
		UploadBuffer->Unmap(0, nullptr);
		UploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount)
{
	// 断言设备驱动是否可用
	assert(InDevice);

	ElementSize = InElementSize;

	// 指定上传堆
	CD3DX12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// 指定资源描述
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InElementCount * InElementCount);
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

