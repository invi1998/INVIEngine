#include "EngineMinimal.h"
#include "NormalBuffer.h"

FNormalBuffer::FNormalBuffer()
{
	// ��ʼ��Buff���ݴ洢��ʽ
	Format = DXGI_FORMAT_R16G16B16A16_FLOAT;	// rgba16 �洢����
}

FNormalBuffer::~FNormalBuffer()
{
}

void FNormalBuffer::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	FRenderBuffer::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FNormalBuffer::SetBufferSize(int wid, int hei)
{
	FRenderBuffer::SetBufferSize(wid, hei);
}

void FNormalBuffer::PreDraw(float DeltaTime)
{
	FRenderBuffer::PreDraw(DeltaTime);
}

void FNormalBuffer::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FRenderBuffer::UpdateCalculations(delta_time, viewport_info);
}

void FNormalBuffer::Build(const XMFLOAT3& center)
{
	FRenderBuffer::Build(center);
}

void FNormalBuffer::Draw(float deltaTime)
{
	FRenderBuffer::Draw(deltaTime);
}

void FNormalBuffer::ResetView(int wid, int hei)
{
	FRenderBuffer::ResetView(wid, hei);
}

void FNormalBuffer::BuildDescriptor()
{
}

void FNormalBuffer::BuildRenderTargetRTV()
{
}

void FNormalBuffer::BuildSRVDescriptor()
{
}

void FNormalBuffer::BuildRTVDescriptor()
{
}

void FNormalBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
	// ��Դ����
	CD3DX12_RESOURCE_DESC CubeMapResourceDesc{};

	memset(&CubeMapResourceDesc, 0, sizeof(CD3DX12_HEAP_PROPERTIES));

	CubeMapResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;		// ��Դά��2D����
	CubeMapResourceDesc.Alignment = 0;		// ���뷽ʽ
	CubeMapResourceDesc.Width = Width;		// ��Դ���
	CubeMapResourceDesc.Height = Height;	// ��Դ�߶�
	CubeMapResourceDesc.DepthOrArraySize = 1;		// ��Դ����
	CubeMapResourceDesc.MipLevels = 1;		// mipmap
	CubeMapResourceDesc.Format = Format;		// ���ݸ�ʽ
	CubeMapResourceDesc.SampleDesc.Count = 1;	// ��������-����-1
	CubeMapResourceDesc.SampleDesc.Quality = 0;	// ��������-����-0
	CubeMapResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// �����ַ�ʽ-δ֪���֣����������о�����
	CubeMapResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // ��Դ���-�����Դ���Ա���������Ϊ���ǵ���ȾĿ�꣨renderTarget��


	// ����Ĭ�϶�
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE ClearValue = CD3DX12_CLEAR_VALUE(Format, Colors::Blue);	// ���ֵ�����Ǹ����߻����������ɫ����Ϊ���ߵ�Ĭ��ֵ��(0,0,1)

	// �����ύ��Դ
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&CubeMapResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&ClearValue,
		IID_PPV_ARGS(OutResource.GetAddressOf())
	));
}
