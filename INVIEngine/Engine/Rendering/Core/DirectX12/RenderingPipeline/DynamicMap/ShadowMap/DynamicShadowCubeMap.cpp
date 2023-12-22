#include "EngineMinimal.h"
#include "DynamicShadowCubeMap.h"

#include "Component/Light/Core/LightComponent.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Manage/LightManager.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/CubeMapRenderTarget.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/Core/RenderTarget.h"


FDynamicShadowCubeMap::FDynamicShadowCubeMap():FDynamicCubeMap()
{
}

FDynamicShadowCubeMap::~FDynamicShadowCubeMap()
{
}

void FDynamicShadowCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
	FRenderLayerManage* inRenderLayer)
{
	FDynamicCubeMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicShadowCubeMap::PreDraw(float DeltaTime)
{
	FDynamicCubeMap::PreDraw(DeltaTime);

	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		int index = 0;
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
				// 1 ���������i RenderTarget��cubeMap�ӿڣ�index �����ǳ�����ĵ��Դ�����ÿ�������6�������
				ViewportAddress += (
					1	// �������
					+ GeometryMap->GetDynamicViewportNum()	// ��̬����������
					+ 1		// shadow
					+ i
					+ index * 6
					) * CBVOffsetSize;
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


			index++;
		}
	}
}

void FDynamicShadowCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	FDynamicCubeMap::UpdateCalculations(delta_time, viewport_info);

	if (CubeMapViewPorts.size() == 6)
	{
		int index = 0;
		for (int k = 0; k < GetLightManger()->GetLights().size(); k++)
		{
			auto meshComponent = GetLightManger()->GetLights()[k];
			if (meshComponent->GetLightType() == ELightType::PointLight)
			{
				XMFLOAT3 position = meshComponent->GetPosition();

				SetViewportPosition(position);
				// �����ӿ�
				for (size_t i = 0; i < 6; i++)
				{
					FViewportInfo tempViewport{};
					XMFLOAT3 position = CubeMapViewPorts[i]->GetPosition();
					tempViewport.CameraPosition = XMFLOAT4{ position.x, position.y, position.z, 1.f };
					tempViewport.ViewMatrix = CubeMapViewPorts[i]->GetViewMatrix();
					tempViewport.ProjectionMatrix = CubeMapViewPorts[i]->GetProjectionMatrix();

					GeometryMap->UpdateCalculationViewport(tempViewport,
						1	// ���ӿ�
						+ GeometryMap->GetDynamicViewportNum()	// ��̬����������
						+ i + index * 6	// CubeMapShadow�������
						);
				}
				index++;
			}
			
		}

	}
}

void FDynamicShadowCubeMap::Build(const XMFLOAT3& center)
{
	FDynamicCubeMap::Build(center);
}

void FDynamicShadowCubeMap::Draw(float deltaTime)
{
	FDynamicCubeMap::Draw(deltaTime);
}

void FDynamicShadowCubeMap::ResetView(int wid, int hei)
{
	FDynamicCubeMap::ResetView(wid, hei);
}

void FDynamicShadowCubeMap::BuildDepthStencilDescriptor()
{
	FDynamicCubeMap::BuildDepthStencilDescriptor();

	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVCubeMapCPUDesc = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),	// DSV����ʼ��ַ
		3,	// ƫ��3 ��һ���Ǹ�����ȾĿ���õ�(������1 + �����1��������������ShadowCubeMap
		DescriptorHandleIncrementSize	// DSVƫ����
	);
}

void FDynamicShadowCubeMap::BuildRenderTargetSRV()
{
	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT CBVDescriptorSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		auto CPUSRVDesHeapStart = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		auto GPUSRVDesHeapStart = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

		// ΪCubeMap����CPU shader��Դ��ͼ
		inRenderTarget->GetCPUShaderResourceView() = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			CPUSRVDesHeapStart,		// CPU SRV����ʼ��ַ
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount() + 1 + 1,	// ƫ���� CubeMap + Shadow
			CBVDescriptorSize	// SRVƫ����
		);

		// ΪCubeMap����GPU shader��Դ��ͼ
		inRenderTarget->GetGPUShaderResourceView() = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			GPUSRVDesHeapStart,		// GPU SRV����ʼ��ַ
			GeometryMap->GetDrawTexture2DCount() + GeometryMap->GetDrawCubeMapCount() + 1 + 1,	// ƫ���� CubeMap + Shadow
			CBVDescriptorSize	// SRVƫ����
		);
	}
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
			inRenderTarget->GetCPURenderTargetView()[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				RTVStart,		// RTV����ʼ��ַ
				FEngineRenderConfig::GetRenderConfig()->SwapChainCount + 6 + i,	// ������ ǰ����Ǹ�����ȾĿ���õ�(������ 6 �Ƿ���� ����Ĳ��Ǹ�ShadowCubeMap�õ�
				RTVSize	// RTVƫ����
			);
		}
	}
}
