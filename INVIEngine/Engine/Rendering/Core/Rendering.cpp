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
	// ��Դ����
	CD3DX12_RESOURCE_DESC BufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);

	// ����Ĭ�϶�
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// �����ύ��Դ
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&BufferResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(Buffer.GetAddressOf())
	));

	// ��ʱ���ϴ�������
	CD3DX12_HEAP_PROPERTIES UpdateBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// �����ύ��Դ
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&UpdateBufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&BufferResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,		// ���ÿɶ����õ�ǰ��Դ����Ⱦ��ˮ���пɶ�����Ϊ�����ʱ�ϴ���������������ˮ��������Ҫ��д���
		nullptr,
		IID_PPV_ARGS(OutTempBuffer.GetAddressOf())
	));

	// ��������Դ
	D3D12_SUBRESOURCE_DATA SubResourceData = {};
	SubResourceData.pData = InData;		// ��Դ����ָ�룬ָ������Ǹ���Դ��ַ
	SubResourceData.RowPitch = InDataSize;		// ��Դ�м�࣬��ȣ�Ҳ������Դ�������С���������ֽ�Ϊ��С��
	SubResourceData.SlicePitch = SubResourceData.RowPitch;		// ������Դ����ȼ�࣬Ҳ���ֽڴ�С

	// �����ԴΪ����Ŀ�� ����Ϊ������Ҫ���ϴ�����Դ���Ƶ�GPU�����Ը��Ƶ�Ŀ����Ҫָ����ǳ�����
	CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		Buffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,		// ��Դ��ǰ״̬
		D3D12_RESOURCE_STATE_COPY_DEST);	// ��ԴĿ��״̬ ָʾ��Դ�������ڴ渴�Ʋ�����Ŀ��

	// ִ����Դ״̬ת������ͬ��GPU
	GetD3dGraphicsCommandList()->ResourceBarrier(1, &CopyDestBarrier);

	// ��������Դ�������
	// �����ݴ� CPU �ڴ渴�Ƶ� GPU �ڴ��е���Դ��ͨ������£�UpdateSubresources ���������������ϴ��ѣ�Upload Heap���еĶ���������������Լ�������Դ��Texture Resource����
	// UpdateSubresources<1> �� Direct3D 12 �е�һ������ģ�壬���ڽ���������Դ�е����ݴ� CPU �ڴ渴�Ƶ� GPU �ڴ��е���Դ��
	// �� UpdateSubresources ������ͬ��UpdateSubresources<1> ����ֱ�Ӳ�����������Դ��������Ҫ���� CopyBufferRegion()��CopyTextureRegion() ���� CopyResource() �Ⱥ�����
	UpdateSubresources<1>(
		GetD3dGraphicsCommandList().Get(),		// ָ��Ҫִ�е������б�
		Buffer.Get(),							// Ŀ����Դ����ָ��
		OutTempBuffer.Get(),					// �м䣨��ʱ����Դ����ָ��
		0,		// �м���Դ��ƫ���������ֽ�Ϊ��λ����
		0,		// ��Դ�е�һ������Դ�������� ��Чֵ��ΧΪ 0 �� MaxSubresources��
		1,		// ����Դ������С��<1>����ػ��У���ֵ�̶�Ϊ1 ��Դ�е�����Դ���� ��Чֵ��ΧΪ 1 �� (MaxSubresources - FirstSubresource) ��
		&SubResourceData		// ��Դ��ַ
	);

	// ��������Դ�󣬶���Դ����״̬ת����������״̬ת��Ϊ�ɶ�״̬
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
		// ����ϴ�������Դ��ӳ�䣨�Ա���CPU�е�ָ��ʧЧ����ӳ�䵽 CPU �ڴ�ռ�Ļ�����������ӳ�䣬��ʹ����Ա� GPU ���ʡ�
		UploadBuffer->Unmap(0, nullptr);
		UploadBuffer = nullptr;
	}
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount)
{
	// �����豸�����Ƿ����
	assert(InDevice);

	ElementSize = InElementSize;

	// ָ���ϴ���
	CD3DX12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ָ����Դ����
	CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InElementCount * InElementCount);
	// �����ύ��Դ
	ANALYSIS_RESULT(InDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&UploadBuffer)
	));

	// ����ӳ��
	ANALYSIS_RESULT(UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&Data)));
}

void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
	// ͨ����ַƫ�ƣ���λ��ָ��������Ԫ�ص��׵�ַ��Ȼ��������Ҫ���µ����ݸ��ڴ濽���ķ�ʽ���������ǵ������У��������Ⱦ���Ԫ�ش�С
	memcpy(&Data[Index * ElementSize], InData, ElementSize);
}

UINT FRenderingResourcesUpdate::GetConstantsBufferByteSize(UINT InTypeSize)
{
	// ��δ������������㳣����������С�ģ����������ǽ������С����ȡ���� 256 �ֽڵ���������������˵����δ���ĺ������£�
	// ���ȣ�����Ҫ����Ŀռ��С���� 255��������Ϊ��Ҫ��������ȡ���������� 255 �� 256 �������������һ��
	// Ȼ�󣬽������ 0xFF00���� 1111 1111 0000 0000����λ�����㣬���� 8 λ���㣬������ȡ���� 256 �ֽڵ�������������ʹ���� ~255 ������ 0xFF00����Ϊ ~255 ��ʾ�� 255 ȡ�����õ��Ķ���������� 0xFF��
	// �ɴ˿ɼ�����δ���������ǽ���Ҫ����Ŀռ��С����ȡ���� 256 �ֽڵ�����������ʵ�ʱ���У����ּ��ɾ��������ڼ�����Դ��С���Ա�֤��Դ���뵽�ض��ı߽磬�Ӷ����Ч�ʲ�������ִ���
	return (InTypeSize + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantsBufferByteSize()
{
	return GetConstantsBufferByteSize(ElementSize);
}

