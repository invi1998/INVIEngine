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
}

void FScreenSpaceAmbientOcclusion::SetBufferSize(int wid, int hei)
{
	NormalBuffer.SetBufferSize(wid, hei);
	AmbientBuffer.SetBufferSize(wid/2, hei/2);
	NoiseBuffer.SetBufferSize(wid, hei);
}

void FScreenSpaceAmbientOcclusion::Build()
{
	SSAORootSignature.BuildRootSignature(1);		// 构建根签名

	BuildSSAOConstantBufferView();	// 构建SSAO常量缓冲视图

	BuildSSAOBlurConstantBuffer();	// 构建SSAO模糊常量缓冲视图

	// 绑定PSO构建代理
	BindBuildPso();
}

void FScreenSpaceAmbientOcclusion::BuildPSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& OutPSODesc)
{
	// 绑定根签名
	OutPSODesc.pRootSignature = SSAORootSignature.GetRootSignature();
}

void FScreenSpaceAmbientOcclusion::BindBuildPso()
{
	if (RenderLayer)
	{
		if (std::shared_ptr<FRenderLayer> layer = RenderLayer->FindByRenderLayer(EMeshRenderLayerType::RENDER_LAYER_SSAO))
		{
			// 绑定委托
			layer->BuildPsoDelegate.Bind(this, &FScreenSpaceAmbientOcclusion::BuildPSO);
		}
		
	}
}

void FScreenSpaceAmbientOcclusion::Draw(float DeltaTime)
{
	NormalBuffer.Draw(DeltaTime);
	AmbientBuffer.Draw(DeltaTime);
	NoiseBuffer.Draw(DeltaTime);

	// 构建SSAO
	// GetD3dGraphicsCommandList()->SetGraphicsRootSignature(SSAORootSignature.GetRootSignature());
	SSAORootSignature.PreDraw(DeltaTime);

	// 渲染资源
	DrawResource();

	// 主SSAO渲染
	if (FBufferRenderTarget* renderTarget = dynamic_cast<FBufferRenderTarget*>(AmbientBuffer.GetRenderTarget().get()))
	{
		

		auto viewport = renderTarget->GetViewport();
		auto rect = renderTarget->GetScissorRect();

		// 设置视口
		GetD3dGraphicsCommandList()->RSSetViewports(1, &viewport);
		GetD3dGraphicsCommandList()->RSSetScissorRects(1, &rect);

		// 将资源从一个状态转换到另一个状态
		CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget->GetRenderTarget(),	//	获取渲染目标
			D3D12_RESOURCE_STATE_GENERIC_READ,	//	从可读状态
			D3D12_RESOURCE_STATE_RENDER_TARGET);	//	到渲染目标状态 转换

		// 清除渲染目标
		GetD3dGraphicsCommandList()->ResourceBarrier(1, &transition);

		const float ClearColor[] = { 1.f, 1.f, 1.f, 1.f };	// 法线的默认值是(0,0,1)
		// 清除渲染目标视图（清空画布）
		GetD3dGraphicsCommandList()->ClearRenderTargetView(
			renderTarget->GetCPURenderTargetView(),
			ClearColor,
			0,
			nullptr);

		// 清除深度模板缓冲区，但是因为我之前已经禁用了深度，所以这里不需要再清除

		// 合并状态，没有深度值，传nullptr
		GetD3dGraphicsCommandList()->OMSetRenderTargets(
			1,
			&renderTarget->GetCPURenderTargetView(),
			true,
			nullptr
		);

		// 渲染SSAO
		RenderLayer->Draw(EMeshRenderLayerType::RENDER_LAYER_SSAO, DeltaTime);

		CD3DX12_RESOURCE_BARRIER transition2 = CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget->GetRenderTarget(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,	//	从渲染状态
			D3D12_RESOURCE_STATE_GENERIC_READ);	//	到可读状态 转换

		GetD3dGraphicsCommandList()->ResourceBarrier(1, &transition2);
	}
}

void FScreenSpaceAmbientOcclusion::DrawResource()
{
	// 刷新绑定常量缓冲区（SSAO常量缓冲区）
	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		0,		// 根签名的0号位置
		SSAOConstantBufferView.GetBuffer()->GetGPUVirtualAddress());	// 常量缓冲区地址

	GetD3dGraphicsCommandList()->SetGraphicsRoot32BitConstant(
		1, // 根签名的1号位置
		0, // 0号位置
		0);	// 值设置为0

	GetD3dGraphicsCommandList()->SetGraphicsRootConstantBufferView(
		2,		// 根签名的2号位置
		SSAOBlurConstantBufferView.GetBuffer()->GetGPUVirtualAddress());	// 常量缓冲区地址

	// 绑定法线（我们之前的 NormalBuffer.Draw(DeltaTime); 已经渲染好我们需要的法线，这里只需要绑定）
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		3,	// 根签名的3号位置
		NormalBuffer.GetRenderTarget()->GetGPUShaderResourceView());

	// 深度
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		4,	// 根签名的9号位置
		DepthBufferRenderTarget->GetGPUShaderResourceView()
	);

	// 噪波
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		5,	// 根签名的1号位置
		NoiseBuffer.GetRenderTarget()->GetGPUShaderResourceView());
}

void FScreenSpaceAmbientOcclusion::DrawSSAOConstantBuffer(float DeltaTime, const FViewportInfo& viewport_info)
{
	FSSAOConstant SSAOConstant;
	// 从视口信息中获取投影矩阵

	// 投影矩阵的逆矩阵
	XMMATRIX invProjection = XMLoadFloat4x4(&SSAOConstant.ProjectionMatrix);	// 加载投影矩阵
	XMVECTOR det = XMMatrixDeterminant(invProjection);	// 计算行列式
	XMMATRIX invView = XMMatrixInverse(&det, invProjection);	// 计算逆矩阵
	XMStoreFloat4x4(&SSAOConstant.InversiveProjectionMatrix, XMMatrixTranspose(invView));		// 转置逆矩阵并存入常量缓冲
	XMStoreFloat4x4(&SSAOConstant.ProjectionMatrix, XMMatrixTranspose(invProjection));		// 转置投影矩阵并存入常量缓冲

	// [-1, 1] -> [0. 1]
	XMMATRIX halfLambertMatrix = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f };

	XMMATRIX textureMatrix = XMMatrixMultiply(invProjection, halfLambertMatrix);	// 投影矩阵和半兰伯特矩阵相乘得到纹理空间矩阵（NDC)
	XMStoreFloat4x4(&SSAOConstant.TextureProjectionMatrix, textureMatrix);			// 存入常量缓冲

	// 随机向量
	SampleVolume.UpdateVolumeData(SSAOConstant.SampleVolumeData);	// 更新采样体积数据

	SSAOConstantBufferView.Update(0, &SSAOConstant);	// 更新常量缓冲
}

void FScreenSpaceAmbientOcclusion::UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info)
{
	NormalBuffer.UpdateCalculations(DeltaTime, viewport_info);
	AmbientBuffer.UpdateCalculations(DeltaTime, viewport_info);
	NoiseBuffer.UpdateCalculations(DeltaTime, viewport_info);

	DrawSSAOConstantBuffer(DeltaTime, viewport_info);
}

void FScreenSpaceAmbientOcclusion::BuildDescriptor()
{
	BuildDepthBuffer();		// 先构建深度缓冲，保证CPU和GPU都能访问到深度缓冲，以及SRV是有效的

	NormalBuffer.BuildDescriptor();
	NormalBuffer.BuildRenderTargetRTVOffset();
	NormalBuffer.BuildSRVDescriptor();
	NormalBuffer.BuildRTVDescriptor();

	NoiseBuffer.BuildDescriptor();
	NoiseBuffer.BuildRenderTargetRTVOffset();
	NoiseBuffer.BuildSRVDescriptor();
	NoiseBuffer.BuildRTVDescriptor();

	AmbientBuffer.SetSRVOffset(GetAmbientSRVOffset());		// 设置环境光SRV偏移
	AmbientBuffer.SetRTVOffset(GetAmbientRTVOffset());		// 设置环境光RTV偏移

	AmbientBuffer.BuildDescriptor();
	AmbientBuffer.BuildRenderTargetRTVOffset();
	AmbientBuffer.BuildSRVDescriptor();
	AmbientBuffer.BuildRTVDescriptor();

	BilateralBlur.SetSRVOffset(GetAmbientSRVOffset());		// 设置双边模糊SRV偏移
	BilateralBlur.SetRTVOffset(GetAmbientRTVOffset());		// 设置双边模糊RTV偏移

	BilateralBlur.BuildDescriptor();
	BilateralBlur.BuildRenderTargetRTVOffset();
	BilateralBlur.BuildSRVDescriptor();
	BilateralBlur.BuildRTVDescriptor();
}

void FScreenSpaceAmbientOcclusion::BuildSSAOConstantBufferView()
{
	SSAOConstantBufferView.CreateConstant(
		sizeof(FSSAOConstant),	// 常量缓冲大小
		1	// 因为我们是一个新的常量缓冲, 所以我们的对象数量是1
	);
}

void FScreenSpaceAmbientOcclusion::BuildSSAOBlurConstantBuffer()
{
	SSAOBlurConstantBufferView.CreateConstant(
		sizeof(FSSAOBlurParam),	// 常量缓冲大小
		1	// 因为我们是一个新的常量缓冲, 所以我们的对象数量是1
	);
}

void FScreenSpaceAmbientOcclusion::SaveSSAOToBuffer()
{
	// 将NormalBuffer保存到帧缓冲（渲染留存）开启这个可以检查NormalBuffer的渲染结果
	//GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
	//	9,	// 根签名的9号位置
	//	NormalBuffer.GetRenderTarget()->GetGPUShaderResourceView()
	//);

	// 将DepthBuffer保存到帧缓冲（渲染留存）开启这个可以检查DepthBuffer的渲染结果
	//GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
	//	9,	// 根签名的9号位置
	//	DepthBufferRenderTarget->GetGPUShaderResourceView()
	//);

	// SSAO保存到帧缓冲（渲染留存）
	GetD3dGraphicsCommandList()->SetGraphicsRootDescriptorTable(
		9,	// 根签名的9号位置
		AmbientBuffer.GetRenderTarget()->GetGPUShaderResourceView()
	);
}

void FScreenSpaceAmbientOcclusion::BuildDepthBuffer()
{
	UINT CBVDescSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	auto CpuSRVDescHeap = GeometryMap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	auto GpuSRVDescHeap = GeometryMap->GetHeap()->GetGPUDescriptorHandleForHeapStart();

	UINT offset = GeometryMap->GetDrawTexture2DCount() +	// 2D纹理数量
		GeometryMap->GetDrawCubeMapCount() +	// 立方体纹理数量
		1 +		// 动态CubeMap数量
		1 +		// shadow直射灯，聚光灯数量
		1 +		// shadow点光源数量
		1 +		// UI
		1 +		// 法线缓冲
		1 + 	// 深度
		1 +		// 噪波
		1;		// SSAO 环境光遮蔽

	// 构建深度缓冲描述堆
	DepthBuffer::BuildDepthBufferDescriptorHeap(CpuSRVDescHeap, GpuSRVDescHeap, CBVDescSize, offset);

	// 构建深度缓冲（这里我们使用的深度资源是我们之前创建的深度资源，也就是主视口的深度信息）
	DepthBuffer::CreateDepthBuffer(GetD3dDevice().Get(), GetDepthBufferResource());
}

UINT FScreenSpaceAmbientOcclusion::GetAmbientSRVOffset() const
{
	UINT offset = GeometryMap->GetDrawTexture2DCount() + // 纹理贴图数量
		GeometryMap->GetDrawCubeMapCount() +	// CubeMap数量
		1 + // 反射Cubemap 动态反射
		1 +	// 阴影贴图 直射灯，聚光灯
		1 + // shadowCubeMap 6个面 (点光源阴影）
		1 + // UI
		1 + // 法线
		1 + // 深度
		1	// 噪波
		;

	return offset;
}

UINT FScreenSpaceAmbientOcclusion::GetAmbientRTVOffset() const
{
	UINT offset = FEngineRenderConfig::GetRenderConfig()->SwapChainCount +	// 获取偏移量 交换链
		6 +	// 反射的CubeMap
		6 + // shadowCubeMap 6个面 (点光源阴影）
		1;	// 法线

	return offset;
}

UINT FScreenSpaceAmbientOcclusion::GetBilateralBlurSRVOffset() const
{
	UINT offset = GeometryMap->GetDrawTexture2DCount() + // 纹理贴图数量
		GeometryMap->GetDrawCubeMapCount() +	// CubeMap数量
		1 + // 反射Cubemap 动态反射
		1 +	// 阴影贴图 直射灯，聚光灯
		1 + // shadowCubeMap 6个面 (点光源阴影）
		1 + // UI
		1 + // 法线
		1 + // 深度
		1 +	// 噪波
		1;	// SSAO 环境光遮蔽
		;

	return offset;
}

UINT FScreenSpaceAmbientOcclusion::GetBilateralBlurRTVOffset() const
{
	UINT offset = FEngineRenderConfig::GetRenderConfig()->SwapChainCount +	// 获取偏移量 交换链
		6 +	// 反射的CubeMap
		6 + // shadowCubeMap 6个面 (点光源阴影）
		1 +	// 法线
		1	// SSAO 环境光遮蔽
		;
	return offset;
}

