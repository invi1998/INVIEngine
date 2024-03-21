#include "EngineMinimal.h"
#include "ScreenSpaceAmbientOcclusion.h"

#include "SSAOConstant.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/DepthBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/NormalBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/RenderLayerManage.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderLayer/Core/RenderLayer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderTarget/BufferRenderTarget.h"

FScreenSpaceAmbientOcclusion::FScreenSpaceAmbientOcclusion(): RenderLayer(nullptr), GeometryMap(nullptr)
{
}

FScreenSpaceAmbientOcclusion::~FScreenSpaceAmbientOcclusion()
{
}

void FScreenSpaceAmbientOcclusion::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer)
{
	RenderLayer = inRenderLayer;

	GeometryMap = inGeometryMap;

	NoiseBuffer.Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
	NormalBuffer.Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
	AmbientBuffer.Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
	BilateralBlur.Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FScreenSpaceAmbientOcclusion::SetBufferSize(int wid, int hei)
{
	NormalBuffer.SetBufferSize(wid, hei);
	AmbientBuffer.SetBufferSize(wid/2, hei/2);
	NoiseBuffer.SetBufferSize(wid, hei);
	BilateralBlur.SetBufferSize(wid, hei);
}

void FScreenSpaceAmbientOcclusion::Build()
{
	SSAORootSignature.BuildRootSignature(1);		// ������ǩ��

	BuildSSAOConstantBufferView();	// ����SSAO����������ͼ

	BuildSSAOBlurConstantBuffer();	// ����SSAOģ������������ͼ

	// ��PSO��������
	BindBuildPso();

	BuildBlurWeight(2.5f);	// ����ģ��Ȩ��
}

void FScreenSpaceAmbientOcclusion::BuildPSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& OutPSODesc)
{
	// �󶨸�ǩ��
	OutPSODesc.pRootSignature = SSAORootSignature.GetRootSignature();
}

void FScreenSpaceAmbientOcclusion::BindBuildPso()
{
	if (RenderLayer)
	{
		if (std::shared_ptr<FRenderLayer> layer = RenderLayer->FindByRenderLayer(EMeshRenderLayerType::RENDER_LAYER_SSAO))
		{
			// ��ί��
			layer->BuildPsoDelegate.Bind(this, &FScreenSpaceAmbientOcclusion::BuildPSO);
		}
		
	}
}

void FScreenSpaceAmbientOcclusion::Draw(float DeltaTime)
{
	NormalBuffer.Draw(DeltaTime);
	AmbientBuffer.Draw(DeltaTime);
	NoiseBuffer.Draw(DeltaTime);

	// ����SSAO
	// GetD3dGraphicsCommandList()->SetGraphicsRootSignature(SSAORootSignature.GetRootSignature());
	SSAORootSignature.PreDraw(DeltaTime);

	// ��Ⱦ��Դ
	DrawResource();

	// ��SSAO��Ⱦ
	DrawSSAO(DeltaTime);

	// ˫��ģ��
	DrawBilateralBlur(DeltaTime, 2);
}

void FScreenSpaceAmbientOcclusion::DrawResource()
{
	// ˢ�°󶨳�����������SSAO������������
	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		0,		// ��ǩ����0��λ��
		SSAOConstantBufferView.GetBuffer()->GetGPUVirtualAddress());	// ������������ַ

	GetD3dGraphicsCommandList()->SetGraphicsRoot32BitConstant(
		1, // ��ǩ����1��λ��
		0, // 0��λ��
		0);	// ֵ����Ϊ0

	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		2,		// ��ǩ����2��λ��
		SSAOBlurConstantBufferView.GetBuffer()->GetGPUVirtualAddress());	// ������������ַ

	// �󶨷��ߣ�����֮ǰ�� NormalBuffer.Draw(DeltaTime); �Ѿ���Ⱦ��������Ҫ�ķ��ߣ�����ֻ��Ҫ�󶨣�
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		3,	// ��ǩ����3��λ��
		NormalBuffer.GetRenderTarget()->GetGPUShaderResourceView());

	// ���
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		4,	// ��ǩ����9��λ��
		DepthBufferRenderTarget->GetGPUShaderResourceView()
	);

	// �벨
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		5,	// ��ǩ����1��λ��
		NoiseBuffer.GetRenderTarget()->GetGPUShaderResourceView());
}

void FScreenSpaceAmbientOcclusion::DrawSSAO(float DeltaTime)
{
	auto viewport = AmbientBuffer.GetRenderTarget().get()->GetViewport();
	auto rect = AmbientBuffer.GetRenderTarget().get()->GetScissorRect();

	// �����ӿ�
	GetD3dGraphicsCommandList()->RSSetViewports(1, &viewport);
	GetD3dGraphicsCommandList()->RSSetScissorRects(1, &rect);

	// ����Դ��һ��״̬ת������һ��״̬
	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(
		AmbientBuffer.GetRenderTarget().get()->GetRenderTarget(),	//	��ȡ��ȾĿ��
		D3D12_RESOURCE_STATE_GENERIC_READ,	//	�ӿɶ�״̬
		D3D12_RESOURCE_STATE_RENDER_TARGET);	//	����ȾĿ��״̬ ת��

	// �����ȾĿ��
	GetD3dGraphicsCommandList()->ResourceBarrier(1, &transition);

	const float ClearColor[] = { 1.f, 1.f, 1.f, 1.f };	// ���ߵ�Ĭ��ֵ��(0,0,1)
	// �����ȾĿ����ͼ����ջ�����
	GetD3dGraphicsCommandList()->ClearRenderTargetView(
		AmbientBuffer.GetRenderTarget().get()->GetCPURenderTargetView(),
		ClearColor,
		0,
		nullptr);

	// ������ģ�建������������Ϊ��֮ǰ�Ѿ���������ȣ��������ﲻ��Ҫ�����

	// �ϲ�״̬��û�����ֵ����nullptr
	GetD3dGraphicsCommandList()->OMSetRenderTargets(
		1,
		&AmbientBuffer.GetRenderTarget().get()->GetCPURenderTargetView(),
		true,
		nullptr
	);

	// ��ȾSSAO
	RenderLayer->Draw(EMeshRenderLayerType::RENDER_LAYER_SSAO, DeltaTime);

	CD3DX12_RESOURCE_BARRIER transition2 = CD3DX12_RESOURCE_BARRIER::Transition(
		AmbientBuffer.GetRenderTarget().get()->GetRenderTarget(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,	//	����Ⱦ״̬
		D3D12_RESOURCE_STATE_GENERIC_READ);	//	���ɶ�״̬ ת��

	GetD3dGraphicsCommandList()->ResourceBarrier(1, &transition2);
	
}

void FScreenSpaceAmbientOcclusion::DrawSSAOConstantBuffer(float DeltaTime, const FViewportInfo& viewport_info)
{
	FSSAOConstant SSAOConstant;
	// ���ӿ���Ϣ�л�ȡͶӰ����

	// ͶӰ����������
	XMMATRIX invProjection = XMLoadFloat4x4(&SSAOConstant.ProjectionMatrix);	// ����ͶӰ����
	XMVECTOR det = XMMatrixDeterminant(invProjection);	// ��������ʽ
	XMMATRIX invView = XMMatrixInverse(&det, invProjection);	// ���������
	XMStoreFloat4x4(&SSAOConstant.InversiveProjectionMatrix, XMMatrixTranspose(invView));		// ת������󲢴��볣������
	XMStoreFloat4x4(&SSAOConstant.ProjectionMatrix, XMMatrixTranspose(invProjection));		// ת��ͶӰ���󲢴��볣������

	// [-1, 1] -> [0. 1]
	XMMATRIX halfLambertMatrix = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f };

	XMMATRIX textureMatrix = XMMatrixMultiply(invProjection, halfLambertMatrix);	// ͶӰ����Ͱ������ؾ�����˵õ�����ռ����NDC)
	XMStoreFloat4x4(&SSAOConstant.TextureProjectionMatrix, textureMatrix);			// ���볣������

	// �������
	SampleVolume.UpdateVolumeData(SSAOConstant.SampleVolumeData);	// ���²����������

	SSAOConstantBufferView.Update(0, &SSAOConstant);	// ���³�������
}

void FScreenSpaceAmbientOcclusion::DrawBlurConstantBuffer(float DeltaTime, const FViewportInfo& viewport_info)
{
	FSSAOBlurParam SSAOBlurParam;

	SSAOBlurParam.BlurWeight[0] = XMFLOAT4(&BlurWeights[0]);
	SSAOBlurParam.BlurWeight[1] = XMFLOAT4(&BlurWeights[4]);
	SSAOBlurParam.BlurWeight[2] = XMFLOAT4(&BlurWeights[8]);

	SSAOBlurParam.InversionTextureSize = XMFLOAT2(1.0f / BilateralBlur.GetWidth(), 1.0f / BilateralBlur.GetHeight());

	SSAOBlurConstantBufferView.Update(0, &SSAOBlurParam);	// ���³�������
}

void FScreenSpaceAmbientOcclusion::UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info)
{
	NormalBuffer.UpdateCalculations(DeltaTime, viewport_info);
	AmbientBuffer.UpdateCalculations(DeltaTime, viewport_info);
	NoiseBuffer.UpdateCalculations(DeltaTime, viewport_info);
	BilateralBlur.UpdateCalculations(DeltaTime, viewport_info);

	DrawSSAOConstantBuffer(DeltaTime, viewport_info);
	DrawBlurConstantBuffer(DeltaTime, viewport_info);
}

void FScreenSpaceAmbientOcclusion::DrawBilateralBlur(float DeltaTime, const UINT DrawTimes)
{
	for (UINT i = 0; i < DrawTimes; ++i)
	{
		DrawBilateralBlurHorizontal(DeltaTime);
		DrawBilateralBlurVertical(DeltaTime);
	}
}

void FScreenSpaceAmbientOcclusion::DrawBilateralBlurHorizontal(float DeltaTime)
{
	DrawBlur(DeltaTime, true);
}

void FScreenSpaceAmbientOcclusion::DrawBilateralBlurVertical(float DeltaTime)
{
	DrawBlur(DeltaTime, false);
}

void FScreenSpaceAmbientOcclusion::DrawBlur(float DeltaTime, bool bHorizontal)
{
	SetRoot32BitConstants(bHorizontal);	// ֪ͨshader��ˮƽ���Ǵ�ֱģ��

	ID3D12Resource* resource = GetDrawResource(bHorizontal);	// ��ȡ��Դ

	CD3DX12_CPU_DESCRIPTOR_HANDLE* rtv = GetDrawRTVResource(bHorizontal);	// ��ȡRTV��Դ

	// ����Դ��һ��״̬ת������һ��״̬
	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(
		resource,	//	��ȡ��ȾĿ��
		D3D12_RESOURCE_STATE_GENERIC_READ,	//	�ӿɶ�״̬
		D3D12_RESOURCE_STATE_RENDER_TARGET);	//	����ȾĿ��״̬ ת��

	// �����ȾĿ��
	GetD3dGraphicsCommandList()->ResourceBarrier(1, &transition);

	const float ClearColor[] = { 1.f, 1.f, 1.f, 1.f };	// ���ߵ�Ĭ��ֵ��(0,0,1)
	// �����ȾĿ����ͼ����ջ�����
	GetD3dGraphicsCommandList()->ClearRenderTargetView(
		*rtv,
		ClearColor,
		0,
		nullptr);

	// ������ģ�建������������Ϊ��֮ǰ�Ѿ���������ȣ��������ﲻ��Ҫ�����

	// �ϲ�״̬��û�����ֵ����nullptr
	GetD3dGraphicsCommandList()->OMSetRenderTargets(
		1,
		rtv,
		true,
		nullptr
	);

	//�󶨽��ܵĻ����� ��srv (shader�Ĵ���������Դ�ı�ţ�
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		6,	// ��ǩ����6��λ��
		*GetDrawSRVResource(bHorizontal));

	// ��ȾSSAO
	RenderLayer->Draw(EMeshRenderLayerType::RENDER_LAYER_SSAO_BILATERAL_BLUR, DeltaTime);

	CD3DX12_RESOURCE_BARRIER transition2 = CD3DX12_RESOURCE_BARRIER::Transition(
		resource,
		D3D12_RESOURCE_STATE_RENDER_TARGET,	//	����Ⱦ״̬
		D3D12_RESOURCE_STATE_GENERIC_READ);	//	���ɶ�״̬ ת��

	GetD3dGraphicsCommandList()->ResourceBarrier(1, &transition2);

}

ID3D12Resource* FScreenSpaceAmbientOcclusion::GetDrawResource(bool bHorizontal)
{
	return bHorizontal ? BilateralBlur.GetRenderTarget()->GetRenderTarget() : AmbientBuffer.GetRenderTarget()->GetRenderTarget();
}

CD3DX12_GPU_DESCRIPTOR_HANDLE* FScreenSpaceAmbientOcclusion::GetDrawSRVResource(bool bHorizontal)
{
	return &(bHorizontal ? AmbientBuffer.GetRenderTarget()->GetGPUShaderResourceView() : BilateralBlur.GetRenderTarget()->GetGPUShaderResourceView());
}

CD3DX12_CPU_DESCRIPTOR_HANDLE* FScreenSpaceAmbientOcclusion::GetDrawRTVResource(bool bHorizontal)
{
	return &(bHorizontal ? BilateralBlur.GetRenderTarget()->GetCPURenderTargetView() : AmbientBuffer.GetRenderTarget()->GetCPURenderTargetView());
}

void FScreenSpaceAmbientOcclusion::SetRoot32BitConstants(bool bHorizontal)
{
	GetD3dGraphicsCommandList()->SetGraphicsRoot32BitConstant(1, static_cast<UINT>(bHorizontal), 0);
}

void FScreenSpaceAmbientOcclusion::BuildDescriptor()
{
	BuildDepthBuffer();		// �ȹ�����Ȼ��壬��֤CPU��GPU���ܷ��ʵ���Ȼ��壬�Լ�SRV����Ч��

	NormalBuffer.SetSRVOffset(GetNormalBufferSRVOffset());		// ���û�����SRVƫ��
	NormalBuffer.SetRTVOffset(GetNormalBufferRTVOffset());		// ���û�����RTVƫ��
	NormalBuffer.BuildDescriptor();
	NormalBuffer.BuildRenderTargetRTVOffset();
	NormalBuffer.BuildSRVDescriptor();
	NormalBuffer.BuildRTVDescriptor();

	NoiseBuffer.SetSRVOffset(GetNoiseBufferSRVOffset());		// �����벨SRVƫ��
	NoiseBuffer.BuildDescriptor();
	NoiseBuffer.BuildRenderTargetRTVOffset();
	NoiseBuffer.BuildSRVDescriptor();
	NoiseBuffer.BuildRTVDescriptor();

	AmbientBuffer.SetSRVOffset(GetAmbientSRVOffset());		// ���û�����SRVƫ��
	AmbientBuffer.SetRTVOffset(GetAmbientRTVOffset());		// ���û�����RTVƫ��
	AmbientBuffer.BuildDescriptor();
	AmbientBuffer.BuildRenderTargetRTVOffset();
	AmbientBuffer.BuildSRVDescriptor();
	AmbientBuffer.BuildRTVDescriptor();

	BilateralBlur.SetSRVOffset(GetBilateralBlurSRVOffset());		// ����˫��ģ��SRVƫ��
	BilateralBlur.SetRTVOffset(GetBilateralBlurRTVOffset());		// ����˫��ģ��RTVƫ��
	BilateralBlur.BuildDescriptor();
	BilateralBlur.BuildRenderTargetRTVOffset();
	BilateralBlur.BuildSRVDescriptor();
	BilateralBlur.BuildRTVDescriptor();
}

void FScreenSpaceAmbientOcclusion::BuildSSAOConstantBufferView()
{
	SSAOConstantBufferView.CreateConstant(
		sizeof(FSSAOConstant),	// ���������С
		1	// ��Ϊ������һ���µĳ�������, �������ǵĶ���������1
	);
}

void FScreenSpaceAmbientOcclusion::BuildSSAOBlurConstantBuffer()
{
	SSAOBlurConstantBufferView.CreateConstant(
		sizeof(FSSAOBlurParam),	// ���������С
		1	// ��Ϊ������һ���µĳ�������, �������ǵĶ���������1
	);
}

void FScreenSpaceAmbientOcclusion::SaveSSAOToBuffer()
{
	// ��NormalBuffer���浽֡���壨��Ⱦ���棩����������Լ��NormalBuffer����Ⱦ���
	//GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
	//	9,	// ��ǩ����9��λ��
	//	NormalBuffer.GetRenderTarget()->GetGPUShaderResourceView()
	//);

	// ��DepthBuffer���浽֡���壨��Ⱦ���棩����������Լ��DepthBuffer����Ⱦ���
	//GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
	//	9,	// ��ǩ����9��λ��
	//	DepthBufferRenderTarget->GetGPUShaderResourceView()
	//);

	// SSAO���浽֡���壨��Ⱦ���棩
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		9,	// ��ǩ����9��λ��
		AmbientBuffer.GetRenderTarget()->GetGPUShaderResourceView()
	);
}

void FScreenSpaceAmbientOcclusion::BuildDepthBuffer()
{
	// ������Ȼ���������
	DepthBuffer::BuildDepthBufferDescriptorHeap(
		GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart(),	// ��ȡCPU���������
		GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart(),	// ��ȡGPU���������
		GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),	// ��ȡCBV/SRV/UAV��������С
		GetDepthBufferSRVOffset());	// ��ȡ��Ȼ���SRVƫ��

	// ������Ȼ��壨��������ʹ�õ������Դ������֮ǰ�����������Դ��Ҳ�������ӿڵ������Ϣ��
	DepthBuffer::CreateDepthBuffer(GetD3dDevice().Get(), GetDepthBufferResource());
}


UINT FScreenSpaceAmbientOcclusion::GetNormalBufferSRVOffset() const
{
	return  GeometryMap->GetDrawTexture2DCount() + //Texture2D
		GeometryMap->GetDrawCubeMapCount() + //��̬Cube��ͼ ���� �����
		1 + //��̬Cube��ͼ ����
		1 + //Shadow ֱ��� �۹�� Shadow
		1 + //ShadowCubeMap ���Դ�� Shadow
		1;//UI
}

UINT FScreenSpaceAmbientOcclusion::GetDepthBufferSRVOffset() const
{
	return	GeometryMap->GetDrawTexture2DCount() + // ������ͼ����
		GeometryMap->GetDrawCubeMapCount() +	// CubeMap����
		1 + //��̬Cube��ͼ ����
		1 + //Shadow ֱ��� �۹�� Shadow
		1 + //ShadowCubeMap ���Դ�� Shadow
		1 + //UI
		1;  //Nor
}

UINT FScreenSpaceAmbientOcclusion::GetNoiseBufferSRVOffset() const
{
	return  GeometryMap->GetDrawTexture2DCount() + //Texture2D
		GeometryMap->GetDrawCubeMapCount() + //��̬Cube��ͼ ���� �����
		1 + //��̬Cube��ͼ ����
		1 + //Shadow ֱ��� �۹�� Shadow
		1 + //ShadowCubeMap ���Դ�� Shadow
		1 + //UI
		1 + //����
		1; //��� 
}

UINT FScreenSpaceAmbientOcclusion::GetAmbientSRVOffset() const
{
	const UINT offset = GeometryMap->GetDrawTexture2DCount() + // ������ͼ����
		GeometryMap->GetDrawCubeMapCount() +	// CubeMap����
		1 + // ����Cubemap ��̬����
		1 +	// ��Ӱ��ͼ ֱ��ƣ��۹��
		1 + // shadowCubeMap 6���� (���Դ��Ӱ��
		1 + // UI
		1 + // ����
		1 + // ���
		1	// �벨
		;

	return offset;
}

UINT FScreenSpaceAmbientOcclusion::GetBilateralBlurSRVOffset() const
{
	const UINT offset = GeometryMap->GetDrawTexture2DCount() + // ������ͼ����
		GeometryMap->GetDrawCubeMapCount() +	// CubeMap����
		1 + // ����Cubemap ��̬����
		1 +	// ��Ӱ��ͼ ֱ��ƣ��۹��
		1 + // shadowCubeMap 6���� (���Դ��Ӱ��
		1 + // UI
		1 + // ����
		1 + // ���
		1 +	// �벨
		1;	// SSAO �������ڱ�
		;

	return offset;
}

UINT FScreenSpaceAmbientOcclusion::GetNormalBufferRTVOffset() const
{
	return	FEngineRenderConfig::GetRenderConfig()->SwapChainCount +//������
		6 +//�����CubeMap RTV
		6; //ShadowCubeMap RTV Point Light
}

UINT FScreenSpaceAmbientOcclusion::GetAmbientRTVOffset() const
{
	const UINT offset = FEngineRenderConfig::GetRenderConfig()->SwapChainCount +	// ��ȡƫ���� ������
		6 +	// �����CubeMap
		6 + // shadowCubeMap 6���� (���Դ��Ӱ��
		1;	// ����

	return offset;
}

UINT FScreenSpaceAmbientOcclusion::GetBilateralBlurRTVOffset() const
{
	const UINT offset = FEngineRenderConfig::GetRenderConfig()->SwapChainCount +	// ��ȡƫ���� ������
		6 +	// �����CubeMap
		6 + // shadowCubeMap 6���� (���Դ��Ӱ��
		1 +	// ����
		1	// SSAO �������ڱ�
		;
	return offset;
}

void FScreenSpaceAmbientOcclusion::BuildBlurWeight(float sigam, bool bRebuild)
{
	if (bRebuild)
	{
		BlurWeights.clear();
	}

	if (BlurWeights.empty())
	{
		// ������̬�ֲ���ģ��Ȩ��
		const int blurRadius = ceil(2.0f * sigam);		// ģ���뾶

		BlurWeights.resize(2 * blurRadius + 1);	// �������ô�С, 2 * blurRadius + 1����Ϊ������Ҫ�������ߵ�Ȩ�أ�������2 * blurRadius + 1��+1����Ϊ�����Ǵ�0��ʼ�ģ�������Ҫ+1

		float Weights = 0.0f;	// Ȩ��
		for (int i = -blurRadius; i <= blurRadius; ++i)
		{
			const float x = static_cast<float>(i);
			BlurWeights[i + blurRadius] = expf(-x * x / (2.0f * sigam * sigam));	// ��̬�ֲ�
			// expf(-x * x / (2.0f * sigam * sigam)) Ϊ��̬�ֲ��Ĺ�ʽ�� xΪ���룬sigamΪ��׼�expfΪe��x�η�,����ֵ��һ��������
			Weights += BlurWeights[i + blurRadius];	// Ȩ���ۼ�
		}

		// Ȩ��ӳ�䵽[0, 1]
		for (int i = 0; i < BlurWeights.size(); ++i)
		{
			BlurWeights[i] /= Weights;
		}
	}
}

