#include "EngineMinimal.h"
#include "NormalBuffer.h"

#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

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
	if (FBufferRenderTarget* renderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		auto RenderTargetViewport = renderTarget->GetViewport();		// ��ȡ�ӿ�
		auto RenderTargetScissorRect = renderTarget->GetScissorRect();	// ��ȡ�ü�����

		GetD3dGraphicsCommandList()->RSSetViewports(1, &RenderTargetViewport);	// �����ӿ�
		GetD3dGraphicsCommandList()->RSSetScissorRects(1, &RenderTargetScissorRect);	// ���òü�����

		// ����Դ״̬�ӿɶ��л�����ȾĿ��
		CD3DX12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);

		// �����ȾĿ����ͼ����ջ�����
		GetD3dGraphicsCommandList()->ClearRenderTargetView(
			renderTarget->GetCPURenderTargetView(),
			Colors::Blue,
			0,
			nullptr);

		// ��ȡ���ģ��(�������ǲ���Ҫ��ƫ�ƣ���Ϊ�����ģ���ǰ������ӿ���ƫ�Ƶģ����ǻ�ȡ��ԭʼ�����ģ����У�
		auto DepthStenciView = GetDSVHeap()->GetCPUDescriptorHandleForHeapStart();

		// ������ģ�建����
		GetD3dGraphicsCommandList()->ClearDepthStencilView(
			DepthStenciView,
			D3D12_CLEAR_FLAG_DEPTH|D3D12_CLEAR_FLAG_STENCIL,
			1.f,
			0,
			0,
			nullptr
		);

		// ��Ⱦ���ӿ�
		GeometryMap->DrawViewport(deltaTime);

		// ����PSO
		RenderLayers->ResetPSO(RENDER_LAYER_SCREEN_NORMAL);

		// �������ģ��(�����ǵ����ģ������Ϊ��ȾĿ��)
		GetD3dGraphicsCommandList()->OMSetRenderTargets(
			1,
			&renderTarget->GetCPURenderTargetView(),
			true,
			&DepthStenciView
		);

		// ��Ⱦģ�ͣ�ִ��shader��
		RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_OPAQUE, ERenderCondition::RC_Shadow);	// ��Ⱦ��͸������
		RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_TRANSPARENT, ERenderCondition::RC_Shadow);	// ��Ⱦ͸������
		RenderLayers->DrawMesh(deltaTime, RENDER_LAYER_OPAQUE_REFLECT, ERenderCondition::RC_Shadow);	// ��Ⱦ��������
		

		// ����Դ״̬��ԭ�ؿɶ�״̬
		CD3DX12_RESOURCE_BARRIER ResourceBarrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_GENERIC_READ
			
		);

	}
}

void FNormalBuffer::ResetView(int wid, int hei)
{
	FRenderBuffer::ResetView(wid, hei);
}

void FNormalBuffer::BuildDescriptor()
{
}

void FNormalBuffer::BuildRenderTargetRTVOffset()
{
	UINT rtvDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);	// ��ȡRTV��������С

	D3D12_CPU_DESCRIPTOR_HANDLE rtvDesHandle = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();	// ��ȡRTV���������

	int offset = FEngineRenderConfig::GetRenderConfig()->SwapChainCount +	// ��ȡƫ���� ������
		6 +	// �����CubeMap
		6;	// shadowCubeMap 6���� (���Դ��Ӱ��

	if (FBufferRenderTarget* renderTarget = dynamic_cast<FBufferRenderTarget*>(RenderTarget.get()))
	{
		renderTarget->GetCPURenderTargetView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvDesHandle, offset, rtvDescriptorSize);	// ����RTV�������������RTV���������ָ��RTV�ѵ�ָ��ƫ��λ��
	}
}

void FNormalBuffer::BuildSRVDescriptor()
{
}

void FNormalBuffer::BuildRTVDescriptor()
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

void FNormalBuffer::BuildRenderTargetBuffer(ComPtr<ID3D12Resource>& OutResource)
{
	OutResource->SetName(L"NormalBuffer");
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
