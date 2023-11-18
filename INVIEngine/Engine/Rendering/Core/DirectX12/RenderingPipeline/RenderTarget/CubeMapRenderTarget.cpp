#include "EngineMinimal.h"
#include "CubeMapRenderTarget.h"

FCubeMapRenderTarget::FCubeMapRenderTarget(): Width(256), Height(256), Format(DXGI_FORMAT_R8G8B8A8_UNORM)
{
	CPURenderTargetView.resize(6);

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

	BuildRenderTarget();

	BuildSRVDescriptor();

	BuildRTVDescriptor();

	
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

void FCubeMapRenderTarget::BuildSRVDescriptor()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC SRV_Desc{};

	SRV_Desc.Format = Format;		// �����������ݸ�ʽ
	SRV_Desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;	// ����SRV��ά�� CUBE_MAP
	SRV_Desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;		// ��ɫ�����ӳ��Ľṹ�塣����������ָ����ͬ��ɫͨ��֮���ӳ���ϵ��
	// D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING��DXGI_ENUM�е�һ����������������Ĭ�ϵ���ɫ�����ӳ�䷽ʽ������أ�����ʾ��RGBA�ĸ���ɫͨ���ֱ�ӳ�䵽�������ݵ�RGBA�ĸ���ɫͨ�� (0, 1, 2, 3)

	// ����cube map
	SRV_Desc.TextureCube.MipLevels = 1;
	SRV_Desc.TextureCube.MostDetailedMip = 0;
	SRV_Desc.TextureCube.ResourceMinLODClamp = 0.f;


	GetD3dDevice()->CreateShaderResourceView(
		GetRenderTarget(),
		&SRV_Desc,
		CPUShaderResourceView
	);
}

void FCubeMapRenderTarget::BuildRTVDescriptor()
{
	for (size_t i = 0; i < 6; i++)
	{
		D3D12_RENDER_TARGET_VIEW_DESC RTV_Desc{};

		RTV_Desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;	// ָ����Դ��ͼά��
		RTV_Desc.Format = Format;	// ���ݸ�ʽ

		// ����Texture2DArray
		RTV_Desc.Texture2DArray.MipSlice = 0;	// MIP_MAP��Ƭ(0,ֻ��һ��mip map��
		RTV_Desc.Texture2DArray.PlaneSlice = 0;
		// ����mip����map��Դ�Ĺ�ʽ
		// MipSlice + ArraySize * MipLevels
		RTV_Desc.Texture2DArray.FirstArraySlice = i;
		RTV_Desc.Texture2DArray.ArraySize = 1;		// ÿһ��RTV��ֻ��һ����ͼ��Դ
		

		GetD3dDevice()->CreateRenderTargetView(
			GetRenderTarget(),
			&RTV_Desc,
			CPURenderTargetView[i]
		);
	}
}

