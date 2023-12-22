#include "EngineMinimal.h"
#include "DynamicShadowCubeMap.h"

#include "Component/Light/Core/LightComponent.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Manage/LightManager.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/Core/RenderTarget.h"


FDynamicShadowCubeMap::FDynamicShadowCubeMap()
{
	CreateRenderTarget<FCubeMapRenderTarget>();
}

FDynamicShadowCubeMap::~FDynamicShadowCubeMap()
{
}

FDynamicShadowCubeMap::FTempViewportCapture::FTempViewportCapture(const XMFLOAT3& InCenterPoint)
{
	BuildViewportCapture(InCenterPoint);
}

void FDynamicShadowCubeMap::FTempViewportCapture::BuildViewportCapture(const XMFLOAT3& InCenterPoint)
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

void FDynamicShadowCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FDynamicMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicShadowCubeMap::PreDraw(float DeltaTime)
{
	FDynamicMap::PreDraw(DeltaTime);

	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		for (int j = 0; j < GetLightManger()->GetLights().size(); j++)
		{
			CLightComponent* tmpLight = GetLightManger()->GetLights()[j];
			if (tmpLight->GetLightType() != ELightType::PointLight)
			{
				break;
			}
			// ָ���ĸ���Դ��ת����״̬����Ϊ����������buffer�������ڲ��Ͻ���
			CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
				inRenderTarget->GetRenderTarget(),				// ��ǰbuffer������
				D3D12_RESOURCE_STATE_GENERIC_READ,		// ��ǰ״̬ ��ʾ��Դ��������������Ŀ��
				D3D12_RESOURCE_STATE_RENDER_TARGET	// Ŀ��״̬ �⽫ʹ��Դ����������ȾĿ�꣬���������Ը���Դִ�г��ֲ�����д��״̬��
			);
			GetD3dGraphicsCommandList()->ResourceBarrier(
				1,
				&ResourceBarrierPresent
			);

			D3D12_VIEWPORT Viewport = inRenderTarget->GetViewport();
			D3D12_RECT Rect = inRenderTarget->GetScissorRect();

			// ���ã����£��ӿ���Ϣ���ü�������Ϣ
			GetD3dGraphicsCommandList()->RSSetViewports(1, &Viewport);
			GetD3dGraphicsCommandList()->RSSetScissorRects(1, &Rect);

			const UINT CBVOffsetSize = GeometryMap->GetViewportConstantBufferViews().GetConstantBufferByteSize();

			for (size_t i = 0; i < 6; i++)
			{
				// �������
				constexpr float ColorBG[] = { 0.1f, 0.105f, 0.11f, 1.0f };
				GetD3dGraphicsCommandList()->ClearRenderTargetView(
					inRenderTarget->GetCPURenderTargetView()[i],		// Ҫ�������ȾĿ����ͼ
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
					&inRenderTarget->GetCPURenderTargetView()[i],		// ָ����ȾĿ��
					true,								// true�������Ǵ���ľ����һ���ڴ�������������ָ��
					&DSVCubeMapCPUDesc		// �������
				);

				// ���²��Ұ�6���ӿ�
				auto ViewportAddress = GeometryMap->GetViewportConstantBufferViewsGPUVirtualAddr();
				// 1 ���������i RenderTarget��cubeMap�ӿڣ�j �����ǳ�����Ķ�̬����������ÿ�������6�������
				ViewportAddress += (1 + i + j * 6) * CBVOffsetSize;
				GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddress);

				// ����PSO
				RenderLayers->ResetPSO(RENDER_LAYER_OPAQUE_SHADOW, EPipelineState::ViewtianeShadow);

				// ��Ⱦ�㼶������ģ��
				RenderLayers->DrawMesh(DeltaTime, RENDER_LAYER_OPAQUE_REFLECT, ERenderCondition::RC_Shadow);
				RenderLayers->DrawMesh(DeltaTime, RENDER_LAYER_OPAQUE, ERenderCondition::RC_Shadow);
				RenderLayers->DrawMesh(DeltaTime, RENDER_LAYER_TRANSPARENT, ERenderCondition::RC_Shadow);
			}



			// ���õ�ǰ������buffer״̬
			CD3DX12_RESOURCE_BARRIER ResourceBarrierRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
				inRenderTarget->GetRenderTarget(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_GENERIC_READ
			);

			
			GetD3dGraphicsCommandList()->ResourceBarrier(
				1,
				&ResourceBarrierRenderTarget
			);

			// ����Ⱦ���ݸ��µ�CubeMap
			GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(6, inRenderTarget->GetGPUShaderResourceView());	// 6�Ǳ��������Ϊ�˹۲���Ⱦ����������CubeMap��Ϣ���µ������۲죩
			GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(8, inRenderTarget->GetGPUShaderResourceView());	// 8����Ӱ��cubeMap



		}
	}
}

void FDynamicShadowCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicMap::UpdateCalculations(delta_time, viewport_info);
}

void FDynamicShadowCubeMap::Build(const XMFLOAT3& center)
{
	FDynamicMap::Build(center);
}

void FDynamicShadowCubeMap::Draw(float deltaTime)
{
	FDynamicMap::Draw(deltaTime);
}

void FDynamicShadowCubeMap::ResetView(int wid, int hei)
{
	FDynamicMap::ResetView(wid, hei);
}

void FDynamicShadowCubeMap::BuildViewPort(const XMFLOAT3& InCenterPoint)
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

void FDynamicShadowCubeMap::BuildDepthStencil()
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

void FDynamicShadowCubeMap::BuildDepthStencilDescriptor()
{
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVCubeMapCPUDesc = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),	// DSV����ʼ��ַ
		1,	// ƫ��1 ��һ���Ǹ�����ȾĿ���õ�(����������Ĳ��Ǹ�cubeMap�õ�
		DescriptorHandleIncrementSize	// DSVƫ����
	);
}

void FDynamicShadowCubeMap::BuildCubeMapRenderTargetDescriptor()
{
	BuildRenderTargetRTV();	// ��ͼ
	BuildRenderTargetSRV();	// shader

	RenderTarget->Init(Width, Height, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void FDynamicShadowCubeMap::SetViewportPosition(const XMFLOAT3& position)
{
	FTempViewportCapture capture{ position };

	for (size_t i = 0; i < 6; i++)
	{
		CubeMapViewPorts[i]->SetPosition(position);
		CubeMapViewPorts[i]->FaceTarget(position, capture.TargetPoint[i], capture.Up[i]);
		CubeMapViewPorts[i]->BuildViewMatrix();
	}
}

bool FDynamicShadowCubeMap::IsExitDynamicReflectionMesh()
{
	return GeometryMap->GetDynamicViewportNum() > 0;
}

void FDynamicShadowCubeMap::BuildRenderTargetRTV()
{
	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT RTVSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		auto RTVStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
		// ΪCubeMap������ȾĿ����ͼ
		for (size_t i = 0; i < 6; i++)
		{
			inRenderTarget->CPURenderTargetView[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				RTVStart,		// RTV����ʼ��ַ
				FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,	// ������ ǰ����Ǹ�����ȾĿ���õ�(����������Ĳ��Ǹ�cubeMap�õ�
				RTVSize	// RTVƫ����
			);
		}
	}
}

void FDynamicShadowCubeMap::BuildRenderTargetSRV()
{
	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT CBVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

		// ΪCubeMap����CPU shader��Դ��ͼ
		inRenderTarget->CPUShaderResourceView = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			CPUSRVDesHeapStart,		// CPU SRV����ʼ��ַ
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount(),	// ƫ����
			CBVDescriptorSize	// SRVƫ����
		);

		// ΪCubeMap����GPU shader��Դ��ͼ
		inRenderTarget->GPUShaderResourceView = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			GPUSRVDesHeapStart,		// GPU SRV����ʼ��ַ
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount(),	// ƫ����
			CBVDescriptorSize	// SRVƫ����
		);
	}
}
