#include "EngineMinimal.h"
#include "AmbientBuffer.h"

#include "Config/EngineRenderConfig.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

FAmbientBuffer::FAmbientBuffer()
{
	Format = DXGI_FORMAT_R16_UNORM;	// r16 �洢������(��������һ����ͨ����ֵ, ǿ��)
}

FAmbientBuffer::~FAmbientBuffer()
{
}

void FAmbientBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FRenderBuffer::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FAmbientBuffer::SetBufferSize(int wid, int hei)
{
	FRenderBuffer::SetBufferSize(wid, hei);
}

void FAmbientBuffer::PreDraw(float DeltaTime)
{
	FRenderBuffer::PreDraw(DeltaTime);
}

void FAmbientBuffer::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FRenderBuffer::UpdateCalculations(delta_time, viewport_info);
}

void FAmbientBuffer::Build(const XMFLOAT3& center)
{
	FRenderBuffer::Build(center);
}

void FAmbientBuffer::Draw(float deltaTime)
{
	FRenderBuffer::Draw(deltaTime);
}

void FAmbientBuffer::ResetView(int wid, int hei)
{
	FRenderBuffer::ResetView(wid, hei);
}

void FAmbientBuffer::BuildDescriptor()
{
}

void FAmbientBuffer::BuildRenderTargetRTVOffset()
{
	UINT rtvDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// ��ȡRTV��������С

	D3D12_CPU_DESCRIPTOR_HANDLE rtvDesHandle = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();	// ��ȡRTV���������

	int offset = FEngineRenderConfig::GetRenderConfig()->SwapChainCount +	// ��ȡƫ���� ������
		6 +	// �����CubeMap
		6 + // shadowCubeMap 6���� (���Դ��Ӱ��
		1;	// ����

	if (FBufferRenderTarget* renderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		renderTarget->GetCPURenderTargetView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvDesHandle, offset, rtvDescriptorSize);	// ����RTV�������������RTV���������ָ��RTV�ѵ�ָ��ƫ��λ��
	}
}

void FAmbientBuffer::BuildSRVDescriptor()
{
}

void FAmbientBuffer::BuildRTVDescriptor()
{
	if (FBufferRenderTarget* renderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};	// ��ȾĿ����ͼ������
		rtvDesc.Format = Format;	// ���ݸ�ʽ
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;	// ��ͼά��-2D����
		rtvDesc.Texture2D.MipSlice = 0;	// �༶��Զ����ĵ�0��mipmap
		rtvDesc.Texture2D.PlaneSlice = 0;	// ����ƽ����Ƭ

		GetD3dDevice()->CreateRenderTargetView(renderTarget->GetRenderTarget(), &rtvDesc, renderTarget->GetCPURenderTargetView());
	}
}

void FAmbientBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
	// ��Դ����
	CD3DX12_RESOURCE_DESC ResourceDesc;

	memset(&ResourceDesc, 0, sizeof(ResourceDesc));

	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;		// ��Դά��2D����
	ResourceDesc.Alignment = 0;		// ���뷽ʽ
	ResourceDesc.Width = Width;		// ��Դ���
	ResourceDesc.Height = Height;	// ��Դ�߶�
	ResourceDesc.DepthOrArraySize = 1;		// ��Դ����
	ResourceDesc.MipLevels = 1;		// mipmap
	ResourceDesc.Format = Format;		// ���ݸ�ʽ
	ResourceDesc.SampleDesc.Count = 1;	// ��������-����-1
	ResourceDesc.SampleDesc.Quality = 0;	// ��������-����-0
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// �����ַ�ʽ-δ֪���֣����������о�����
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // ��Դ���-�����Դ���Ա���������Ϊ���ǵ���ȾĿ�꣨renderTarget��

	// ����Ĭ�϶�
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	const float AmbientColor[] = { 1.f, 1.f, 1.f, 1.f };
	CD3DX12_CLEAR_VALUE ClearValue(Format, AmbientColor);	// ���ֵ

	// �����ύ��Դ
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&ClearValue,
		IID_PPV_ARGS(OutResource.GetAddressOf())
	));
}
