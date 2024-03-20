#include "EngineMinimal.h"
#include "NoiseBuffer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/Core/RenderTarget.h"

FNoiseBuffer::FNoiseBuffer()
{
	Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// r8g8b8a8 �޷�������
}

FNoiseBuffer::~FNoiseBuffer()
{
}

void FNoiseBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FRenderBuffer::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FNoiseBuffer::SetBufferSize(int wid, int hei)
{
	FRenderBuffer::SetBufferSize(wid, hei);
}

void FNoiseBuffer::PreDraw(float DeltaTime)
{
	FRenderBuffer::PreDraw(DeltaTime);
}

void FNoiseBuffer::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FRenderBuffer::UpdateCalculations(delta_time, viewport_info);
}

void FNoiseBuffer::Build(const XMFLOAT3& center)
{
	FRenderBuffer::Build(center);
}

void FNoiseBuffer::Draw(float deltaTime)
{
}

void FNoiseBuffer::ResetView(int wid, int hei)
{
	FRenderBuffer::ResetView(wid, hei);
}

void FNoiseBuffer::BuildDescriptor()
{
	CalculateSRVOffset();
}

void FNoiseBuffer::BuildRenderTargetRTVOffset()
{
}

void FNoiseBuffer::BuildSRVDescriptor()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};	// ��ɫ����Դ��ͼ������
	srvDesc.Format = Format;	// ���ݸ�ʽ
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// ��ͼά��-2D����
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;	// ��ɫ��4����ӳ��

	srvDesc.Texture2D.MostDetailedMip = 0;	// ����ϸ��mipmap
	srvDesc.Texture2D.MipLevels = 1;	// mipmap����

	GetD3dDevice()->CreateShaderResourceView(RenderTarget->GetRenderTarget(), &srvDesc, RenderTarget->GetCPUShaderResourceView());
}

void FNoiseBuffer::BuildRTVDescriptor()
{
}

void FNoiseBuffer::BuildUploadBuffer(const D3D12_RESOURCE_DESC& ResourceDesc, ComPtr<ID3D12Resource>& OutResource)
{
	// �����ϴ���
	CD3DX12_HEAP_PROPERTIES UploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	UINT SubNum = ResourceDesc.MipLevels * ResourceDesc.DepthOrArraySize;
	UINT UploadBufferSize = GetRequiredIntermediateSize(OutResource.Get(), 0, SubNum);

	// ������Դ���� 256 * 256 * 4 * 4
	CD3DX12_RESOURCE_DESC BufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(UploadBufferSize);

	// �����ύ��Դ
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&UploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&BufferResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(NoiseMapUploadBuffer.GetAddressOf())
	));

	 XMCOLOR Buffer[256 * 256] = {0};
	 D3D12_SUBRESOURCE_DATA SubResourceData = GetSubResourceData(Buffer, 256, 256);

	//// �����ݿ������ϴ���
	//float* Data = nullptr;
	//CD3DX12_RANGE ReadRange(0, 0);
	//ANALYSIS_RESULT(NoiseMapUploadBuffer->Map(0, &ReadRange, reinterpret_cast<void**>(&Data)));
	//for (int i = 0; i < ResourceDesc.Width * ResourceDesc.Height * 4; i++)
	//{
	//	Data[i] = static_cast<float>(rand()) / RAND_MAX;
	//}
	//NoiseMapUploadBuffer->Unmap(0, nullptr);

	// �����ݴ��ϴ��ѿ�����Ĭ�϶�
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(OutResource.Get(),
				D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST);
	GetD3dGraphicsCommandList()->ResourceBarrier(1, &Barrier);

	// GetD3dGraphicsCommandList()->CopyResource(OutResource.Get(), NoiseMapUploadBuffer.Get());
	UpdateSubresources(GetD3dGraphicsCommandList().Get(), OutResource.Get(), NoiseMapUploadBuffer.Get(), 0, 0, SubNum, &SubResourceData);

	Barrier = CD3DX12_RESOURCE_BARRIER::Transition(OutResource.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	GetD3dGraphicsCommandList()->ResourceBarrier(1, &Barrier);
}

D3D12_SUBRESOURCE_DATA FNoiseBuffer::GetSubResourceData(XMCOLOR* InBuffer, int w, int h)
{
	D3D12_SUBRESOURCE_DATA SubResourceData = {};
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			InBuffer[j + i * w] = XMCOLOR(
				get_float_random_static(),
				get_float_random_static(),
				get_float_random_static(), 0.f);
		}
	}

	SubResourceData.pData = InBuffer;
	SubResourceData.RowPitch = w * sizeof(XMCOLOR);
	SubResourceData.SlicePitch = SubResourceData.RowPitch * h;

	return SubResourceData;
}

void FNoiseBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
	// ��Դ����
	CD3DX12_RESOURCE_DESC ResourceDesc{};

	memset(&ResourceDesc, 0, sizeof(ResourceDesc));

	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;		// ��Դά��2D����
	ResourceDesc.Alignment = 0;		// ���뷽ʽ
	ResourceDesc.Width = 256;		// ��Դ���
	ResourceDesc.Height = 256;	// ��Դ�߶�
	ResourceDesc.DepthOrArraySize = 1;		// ��Դ����
	ResourceDesc.MipLevels = 1;		// mipmap
	ResourceDesc.Format = Format;		// ���ݸ�ʽ
	ResourceDesc.SampleDesc.Count = 1;	// ��������-����-1
	ResourceDesc.SampleDesc.Quality = 0;	// ��������-����-0
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// �����ַ�ʽ-δ֪���֣����������о�����
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE; // ��Դ���-����Դû�б�ǣ�û��RTV����������CPU�����ɵ��벨ͼ

	// ����Ĭ�϶�
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// �����ύ��Դ
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(OutResource.GetAddressOf())
	));

	BuildUploadBuffer(ResourceDesc, OutResource);
}
