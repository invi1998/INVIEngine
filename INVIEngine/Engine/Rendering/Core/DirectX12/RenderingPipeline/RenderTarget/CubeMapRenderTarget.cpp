#include "EngineMinimal.h"
#include "CubeMapRenderTarget.h"

FCubeMapRenderTarget::FCubeMapRenderTarget(): Width(256), Height(256), Format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
	ResetViewport();

	ResetScissorRect();
}

void FCubeMapRenderTarget::Init(UINT w, UINT h, const DXGI_FORMAT& format)
{
	Width = w;
	Height = h;
	Format = format;

	ResetViewport();

	ResetScissorRect();
}

void FCubeMapRenderTarget::ResetViewport()
{
	Viewport = {
		0.f,
		0.f,
		static_cast<float>(Width),
		static_cast<float>(Height),
		0.f,	// ��С���
		1.f		// ������
	};
}

void FCubeMapRenderTarget::ResetScissorRect()
{
	ScissorRect = {
		0,
		0,
		static_cast<LONG>(Width),
		static_cast<LONG>(Height)
	};
}

void FCubeMapRenderTarget::BuildRenderTarget()
{
	// ��Դ����
	CD3DX12_RESOURCE_DESC CubeMapResourceDesc{};

	memset(&CubeMapResourceDesc, 0, sizeof(CD3DX12_HEAP_PROPERTIES));

	CubeMapResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;		// ��Դά��2D����
	CubeMapResourceDesc.Alignment = 0;		// ���뷽ʽ
	CubeMapResourceDesc.Width = Width;		// ��Դ���
	CubeMapResourceDesc.Height = Height;	// ��Դ�߶�
	CubeMapResourceDesc.DepthOrArraySize = 6;		// ��Դ������6������Ϊ���ǵ�����������ͼ������������Դ��3Dά�ȣ���ô���ֵ����ָ����Դ��ȣ������1D, 2D����ô���ֵ����ָ��Դ����ĳ��ȣ�
	CubeMapResourceDesc.MipLevels = 1;		// mipmap
	CubeMapResourceDesc.Format = Format;		// ���ݸ�ʽ
	CubeMapResourceDesc.SampleDesc.Count = 1;	// ��������-����-1
	CubeMapResourceDesc.SampleDesc.Quality = 0;	// ��������-����-0
	CubeMapResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// �����ַ�ʽ-δ֪���֣����������о�����
	CubeMapResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // ��Դ���-�����Դ���Ա���������Ϊ���ǵ���ȾĿ�꣨renderTarget��


	// ����Ĭ�϶�
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// �����ύ��Դ
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&CubeMapResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(RenderTargetMap.GetAddressOf())
	));
}
