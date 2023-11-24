#include "EngineMinimal.h"
#include "DynamicCubeMap.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"


FDynamicCubeMap::FTempViewportCapture::FTempViewportCapture(const XMFLOAT3& InCenterPoint)
{
	BuildViewportCapture(InCenterPoint);
}

void FDynamicCubeMap::FTempViewportCapture::BuildViewportCapture(const XMFLOAT3& InCenterPoint)
{
	// +x, -x, +y, -y, +z, -z
	// �����������6����
	TargetPoint[0] = XMFLOAT3{ InCenterPoint.x + 1.0f, InCenterPoint.y, InCenterPoint.z };
	TargetPoint[1] = XMFLOAT3{ InCenterPoint.x - 1.0f, InCenterPoint.y, InCenterPoint.z };
	TargetPoint[2] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y + 1.0f, InCenterPoint.z };
	TargetPoint[3] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y - 1.0f, InCenterPoint.z };
	TargetPoint[4] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z + 1.0f };
	TargetPoint[5] = XMFLOAT3{ InCenterPoint.x, InCenterPoint.y, InCenterPoint.z - 1.0f };


	Up[0] = XMFLOAT3{ 0.f, 1.f, 0.f };
	Up[1] = XMFLOAT3{ 0.f, 1.f, 0.f };
	Up[2] = XMFLOAT3{ 0.f, 0.f, -1.f };
	Up[3] = XMFLOAT3{ 0.f, 0.f, 1.f };
	Up[4] = XMFLOAT3{ 0.f, 1.f, 0.f };
	Up[5] = XMFLOAT3{ 0.f, 1.f, 0.f };
}

FDynamicCubeMap::FDynamicCubeMap()
{
	CubeMapRenderTarget = std::make_unique<FCubeMapRenderTarget>();
}

void FDynamicCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	GeometryMap = inGeometryMap;
	DirectXPipelineState = inDirectXPipelineState;
	RenderLayers = inRenderLayer;
}

void FDynamicCubeMap::PreDraw(float DeltaTime)
{
	// ��Ⱦ�ƹ⣬���ʣ���ͼ
	GeometryMap->Draw(DeltaTime);

	// ָ���ĸ���Դ��ת����״̬����Ϊ����������buffer�������ڲ��Ͻ���
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		CubeMapRenderTarget->GetRenderTarget(),				// ��ǰbuffer������
		D3D12_RESOURCE_STATE_GENERIC_READ,		// ��ǰ״̬ ��ʾ��Դ��������������Ŀ��
		D3D12_RESOURCE_STATE_RENDER_TARGET	// Ŀ��״̬ �⽫ʹ��Դ����������ȾĿ�꣬���������Ը���Դִ�г��ֲ�����д��״̬��
	);
	GetD3dGraphicsCommandList()->ResourceBarrier(
		1,
		&ResourceBarrierPresent
	);

	D3D12_VIEWPORT Viewport = CubeMapRenderTarget->GetViewport();
	D3D12_RECT Rect = CubeMapRenderTarget->GetScissorRect();

	// ���ã����£��ӿ���Ϣ���ü�������Ϣ
	GetD3dGraphicsCommandList()->RSSetViewports(1, &Viewport);
	GetD3dGraphicsCommandList()->RSSetScissorRects(1, &Rect);

	const UINT CBVOffsetSize = GeometryMap->ViewportConstantBufferViews.GetConstantBufferByteSize();

	for (size_t i = 0; i < 6; i++)
	{
		// �������
		constexpr float ColorBG[] = { 0.1f, 0.105f, 0.11f, 1.0f };
		GetD3dGraphicsCommandList()->ClearRenderTargetView(
			CubeMapRenderTarget->CPURenderTargetView[i],		// Ҫ�������ȾĿ����ͼ
			ColorBG,		// ������ɫ
			0,		// ���������������Ǻ��ӿ���صģ����ﲻ���������ã��������ר�ŵ����÷���
			nullptr
		);

		// �����Ⱥ�ģ�建����
		GetD3dGraphicsCommandList()->ClearDepthStencilView(
			DSVCubeMapCPUDesc,	// ����CPU�ڴ棨Ҫ��������ģ�建�����ڴ棩
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,		// �����Ⱥ�ģ�建����
			1.f,	// ��1��������ǵ���Ȼ�����������Ȼ���������Ϊ1
			0,		// ��0��������ǵ�ģ�建������ģ�建��������Ϊ0
			0,
			nullptr
		);

		// ָ����Ⱦ������������ϲ��׶Σ�������ȾĿ����ͼ
		GetD3dGraphicsCommandList()->OMSetRenderTargets(
			1,									// ָ����ȾĿ���� 1
			&CubeMapRenderTarget->CPURenderTargetView[i],		// ָ����ȾĿ��
			true,								// true�������Ǵ���ľ����һ���ڴ�������������ָ��
			&DSVCubeMapCPUDesc		// �������
		);

		// ���²��Ұ�6���ӿ�
		auto ViewportAddress = GeometryMap->ViewportConstantBufferViews.GetBuffer()->GetGPUVirtualAddress();
		ViewportAddress += (i + 1) * CBVOffsetSize;
		GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddress);

		// ��Ⱦ�㼶������ģ��
		RenderLayers->Draw(RENDER_LAYER_BACKGROUND, DeltaTime);
		RenderLayers->Draw(RENDER_LAYER_OPAQUE, DeltaTime);
		RenderLayers->Draw(RENDER_LAYER_TRANSPARENT, DeltaTime);
	}

	

	// ���õ�ǰ������buffer״̬
	CD3DX12_RESOURCE_BARRIER ResourceBarrierRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		CubeMapRenderTarget->GetRenderTarget(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_GENERIC_READ
	);
	GetD3dGraphicsCommandList()->ResourceBarrier(
		1,
		&ResourceBarrierRenderTarget
	);

}

void FDynamicCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{

	if (CubeMapViewPorts.size() == 6)
	{
		for (int k = 0; k < GeometryMap->DynamicReflectionMeshComponents.size(); k++)
		{
			auto meshComponent = GeometryMap->DynamicReflectionMeshComponents[k];
			XMFLOAT3 position = meshComponent->GetPosition();

			SetCubeMapViewportPosition(position);
			// �����ӿ�
			for (size_t i = 0; i < 6; i++)
			{
				FViewportInfo tempViewport{};
				XMFLOAT3 position = CubeMapViewPorts[i]->GetPosition();
				tempViewport.CameraPosition = XMFLOAT4{ position.x, position.y, position.z, 1.f};
				tempViewport.ViewMatrix = CubeMapViewPorts[i]->GetViewMatrix();
				tempViewport.ProjectionMatrix = CubeMapViewPorts[i]->GetProjectionMatrix();

				GeometryMap->UpdateCalculationViewport(tempViewport, 
					i + k*6	// ��̬�����6�������
					+1);
			}
		}
		
	}
	
}

void FDynamicCubeMap::Build(const XMFLOAT3& center)
{
	BuildViewPort(center);		// �����ӿ�

	BuildCubeMapRenderTargetDescriptor();		// ����CubeMap��ȾĿ������

	BuildDepthStencilDescriptor();		// �������ģ������
	
	BuildDepthStencil();		// �������ģ��
}

void FDynamicCubeMap::Draw(float deltaTime)
{
	// ����CubeMap
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, CubeMapRenderTarget->GPUShaderResourceView);
}

void FDynamicCubeMap::BuildViewPort(const XMFLOAT3& InCenterPoint)
{
	FTempViewportCapture capture{ InCenterPoint };
	

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts.push_back(CreateObject<GClientViewPort>(new GClientViewPort()));
		GClientViewPort* viewport = CubeMapViewPorts[CubeMapViewPorts.size() - 1];

		viewport->SetPosition(InCenterPoint);
		viewport->FaceTarget(InCenterPoint, capture.TargetPoint[i], capture.Up[i]);
		viewport->SetFrustum(XM_PIDIV2, 1, 1, 0.1f, 10000.f);
		viewport->BuildViewMatrix();
	}
}

void FDynamicCubeMap::BuildDepthStencil()
{
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = Width;
	ResourceDesc.Height = Height;
	ResourceDesc.Alignment = 0;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;
	ClearValue.DepthStencil.Stencil = 0;
	ClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	GetD3dDevice()->CreateCommittedResource(
		&Properties,
		D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_COMMON, &ClearValue,
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

	GetD3dDevice()->CreateDepthStencilView(DepthStencilBuffer.Get(), nullptr, DSVCubeMapCPUDesc);

	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	GetD3dGraphicsCommandList()->ResourceBarrier(1, &Barrier);
}

void FDynamicCubeMap::BuildDepthStencilDescriptor()
{
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVCubeMapCPUDesc = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),	// DSV����ʼ��ַ
		1,	// ƫ��1 ��һ���Ǹ�����ȾĿ���õ�(����������Ĳ��Ǹ�cubeMap�õ�
		DescriptorHandleIncrementSize	// DSVƫ����
	);
}

void FDynamicCubeMap::BuildCubeMapRenderTargetDescriptor()
{

	BuildRenderTargetRTV();	// ��ͼ
	BuildRenderTargetSRV();	// shader

	CubeMapRenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);
	
}

void FDynamicCubeMap::SetCubeMapViewportPosition(const XMFLOAT3& position)
{
	
	FTempViewportCapture capture{ position };

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts[i]->SetPosition(position);
		CubeMapViewPorts[i]->FaceTarget(position, capture.TargetPoint[i], capture.Up[i]);
		CubeMapViewPorts[i]->BuildViewMatrix();
	}
}

void FDynamicCubeMap::BuildRenderTargetRTV()
{
	UINT RTVSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	auto RTVStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
	// ΪCubeMap������ȾĿ����ͼ
	for (size_t i = 0; i < 6; i++)
	{
		CubeMapRenderTarget->CPURenderTargetView[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			RTVStart,		// RTV����ʼ��ַ
			FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,	// ������ ǰ����Ǹ�����ȾĿ���õ�(����������Ĳ��Ǹ�cubeMap�õ�
			RTVSize	// RTVƫ����
		);
	}
}

void FDynamicCubeMap::BuildRenderTargetSRV()
{
	UINT CBVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

	// ΪCubeMap����CPU shader��Դ��ͼ
	CubeMapRenderTarget->CPUShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		CPUSRVDesHeapStart,		// CPU SRV����ʼ��ַ
		GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount(),	// ƫ����
		CBVDescriptorSize	// SRVƫ����
	);

	// ΪCubeMap����GPU shader��Դ��ͼ
	CubeMapRenderTarget->GPUShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		GPUSRVDesHeapStart,		// GPU SRV����ʼ��ַ
		GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount(),	// ƫ����
		CBVDescriptorSize	// SRVƫ����
	);
}
