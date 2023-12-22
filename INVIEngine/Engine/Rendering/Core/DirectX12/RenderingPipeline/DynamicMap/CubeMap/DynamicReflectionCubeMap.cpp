#include "EngineMinimal.h"
#include "DynamicReflectionCubeMap.h"

#include "Component/Mesh/Core/MeshComponent.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Construction/ObjectConstruction.h"
#include "Core/Viewport/ClientViewPort.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"


FDynamicReflectionCubeMap::FDynamicReflectionCubeMap():FDynamicCubeMap()
{
}

void FDynamicReflectionCubeMap::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	FDynamicCubeMap::Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FDynamicReflectionCubeMap::PreDraw(float DeltaTime)
{
	FDynamicCubeMap::PreDraw(DeltaTime);

	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		for (int j = 0; j < GeometryMap->GetDynamicReflectionMeshComponents().size(); j++)
		{
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
				auto ViewportAddress = GeometryMap->GetViewportConstantBufferViews().GetBuffer()->GetGPUVirtualAddress();
				// 1 ���������i RenderTarget��cubeMap�ӿڣ�j �����ǳ�����Ķ�̬����������ÿ�������6�������
				ViewportAddress += (1 + i + j * 6) * CBVOffsetSize;
				GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(1, ViewportAddress);

				// ��Ⱦ�㼶������ģ��
				RenderLayers->Draw(RENDER_LAYER_BACKGROUND, DeltaTime);
				RenderLayers->Draw(RENDER_LAYER_OPAQUE, DeltaTime);
				RenderLayers->Draw(RENDER_LAYER_TRANSPARENT, DeltaTime);
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

			// ׼�����ӿ�
			StartSetMainViewportRenderTarget();

			// ��Ⱦ���ӿ�
			GeometryMap->DrawViewport(DeltaTime);

			Draw(DeltaTime);

			// ����Ⱦ�㼶�����ָ����ģ�ͽ�����Ⱦ����Ϊ��̬���䲻�Ƕ����е�ģ�Ͷ����ж�̬�������ģ�ֻ�Զ�̬������ʵ�ģ�ͽ�����Ⱦ��
			RenderLayers->FindObjectDraw(DeltaTime, RENDER_LAYER_OPAQUE_REFLECT, GeometryMap->GetDynamicReflectionMeshComponents()[j]);

			// ����CubeMap
			GeometryMap->DrawCubeMapTexture(DeltaTime);

			// ������ǰ���ӿ���Ⱦ
			EndSetMainViewportRenderTarget();
		}
	}
}

void FDynamicReflectionCubeMap::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{

	if (CubeMapViewPorts.size() == 6)
	{
		for (int k = 0; k < GeometryMap->GetDynamicReflectionMeshComponents().size(); k++)
		{
			auto meshComponent = GeometryMap->GetDynamicReflectionMeshComponents()[k];
			XMFLOAT3 position = meshComponent->GetPosition();

			SetViewportPosition(position);
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

void FDynamicReflectionCubeMap::Build(const XMFLOAT3& center)
{
	BuildViewPort(center);		// �����ӿ�

	BuildCubeMapRenderTargetDescriptor();		// ����CubeMap��ȾĿ������

	BuildDepthStencilDescriptor();		// �������ģ������

	BuildDepthStencil();		// �������ģ��
}

void FDynamicReflectionCubeMap::BuildDepthStencilDescriptor()
{
	UINT DescriptorHandleIncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	DSVCubeMapCPUDesc = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		GetDSVHeap()->GetCPUDescriptorHandleForHeapStart(),	// DSV����ʼ��ַ
		1,	// ƫ��1 ��һ���Ǹ�����ȾĿ���õ�(����������Ĳ��Ǹ�cubeMap�õ�
		DescriptorHandleIncrementSize	// DSVƫ����
	);
}

void FDynamicReflectionCubeMap::BuildRenderTargetRTV()
{
	FDynamicCubeMap::BuildRenderTargetRTV();

	if (FCubeMapRenderTarget* inRenderTarget = dynamic_cast<FCubeMapRenderTarget*>(this->RenderTarget.get()))
	{
		UINT RTVSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		auto RTVStart = GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
		// ΪCubeMap������ȾĿ����ͼ
		for (size_t i = 0; i < 6; i++)
		{
			inRenderTarget->GetCPURenderTargetView()[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				RTVStart,		// RTV����ʼ��ַ
				FEngineRenderConfig::GetRenderConfig()->SwapChainCount + i,	// ������ ǰ����Ǹ�����ȾĿ���õ�(����������Ĳ��Ǹ�cubeMap�õ�
				RTVSize	// RTVƫ����
			);
		}
	}
}

bool FDynamicReflectionCubeMap::IsExitDynamicReflectionMesh()
{
	return GeometryMap->GetDynamicViewportNum() > 0;
}

