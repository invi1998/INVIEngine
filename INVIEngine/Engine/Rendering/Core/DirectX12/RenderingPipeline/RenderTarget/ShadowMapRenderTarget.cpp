#include "EngineMinimal.h"
#include "ShadowMapRenderTarget.h"

FShadowMapRenderTarget::FShadowMapRenderTarget() : FRenderTarget()
{
}

void FShadowMapRenderTarget::Init(UINT w, UINT h, const DXGI_FORMAT& format)
{
	FRenderTarget::Init(w, h, format);
}

void FShadowMapRenderTarget::BuildRenderTarget()
{
	// ��Դ����
	CD3DX12_RESOURCE_DESC CubeMapResourceDesc{};

	memset(&CubeMapResourceDesc, 0, sizeof(CD3DX12_HEAP_PROPERTIES));

	CubeMapResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;		// ��Դά��2D����
	CubeMapResourceDesc.Alignment = 0;		// ���뷽ʽ
	CubeMapResourceDesc.Width = Width;		// ��Դ���
	CubeMapResourceDesc.Height = Height;	// ��Դ�߶�
	CubeMapResourceDesc.DepthOrArraySize = 1;		// ��Դ������1���������Ӱ��ͼ��
	CubeMapResourceDesc.MipLevels = 1;		// mipmap
	CubeMapResourceDesc.Format = Format;		// ���ݸ�ʽ
	CubeMapResourceDesc.SampleDesc.Count = 1;	// ��������-����-1
	CubeMapResourceDesc.SampleDesc.Quality = 0;	// ��������-����-0
	CubeMapResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;	// �����ַ�ʽ-δ֪���֣����������о�����
	CubeMapResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // ��Դ���-�����Դ���Ա���������Ϊ���ǵ����ģ�����


	// ����Ĭ�϶�
	CD3DX12_HEAP_PROPERTIES BufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	// �����ύ��Դ
	ANALYSIS_RESULT(GetD3dDevice()->CreateCommittedResource(
		&BufferProperties,
		D3D12_HEAP_FLAG_NONE,
		&CubeMapResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		&clearValue,
		IID_PPV_ARGS(RenderTargetMap.GetAddressOf())
	));
}

void FShadowMapRenderTarget::BuildSRVDescriptor()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRV_Desc{};

	SRV_Desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;		// �����������ݸ�ʽ
	SRV_Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;		// ����SRV��ά�� Texture2D
	SRV_Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;		// ��ɫ�����ӳ��Ľṹ�塣����������ָ����ͬ��ɫͨ��֮���ӳ���ϵ��
	// D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING��DXGI_ENUM�е�һ����������������Ĭ�ϵ���ɫ�����ӳ�䷽ʽ������أ�����ʾ��RGBA�ĸ���ɫͨ���ֱ�ӳ�䵽�������ݵ�RGBA�ĸ���ɫͨ�� (0, 1, 2, 3)

	// ����Texture2D
	SRV_Desc.Texture2D.MipLevels = 1;		// mipmap
	SRV_Desc.Texture2D.MostDetailedMip = 0;	// ����ϸ��mipmap
	SRV_Desc.Texture2D.PlaneSlice = 0;		// ƽ����Ƭ


	GetD3dDevice()->CreateShaderResourceView(
		GetRenderTarget(),
		&SRV_Desc,
		CPUShaderResourceView
	);
}

void FShadowMapRenderTarget::BuildRTVDescriptor()
{
}
