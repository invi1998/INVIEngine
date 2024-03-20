#pragma once

#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/ConstantBuffer/ConstantBufferViews.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/AmbientBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/NoiseBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/NormalBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/SampleVolume.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RootSignature/SSAODirectXRootSignature.h"

enum EMeshRenderLayerType : int;
struct FViewportInfo;
struct FDirectXPipelineState;
struct FGeometryMap;

struct FScreenSpaceAmbientOcclusion : public IDirectXDeviceInterface_Struct
{
public:
	FScreenSpaceAmbientOcclusion();
	~FScreenSpaceAmbientOcclusion();

	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer);
	void SetBufferSize(int wid, int hei);

	void Build();	// 构建AO管线
	void BuildPSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& OutPSODesc);	// 构建PSO

	void BindBuildPso();	// 绑定构建PSO

	void Draw(float DeltaTime);
	
	void DrawSSAOConstantBuffer(float DeltaTime, const FViewportInfo& viewport_info);	// 绘制SSAO常量buffer
	void UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info);
	

	void BuildDescriptor();		// 构建描述符 用于绑定到管线上 RenderTargetView ShaderResourceView

	void BuildSSAOConstantBufferView();	// 构建SSAO常量缓冲视图

	void BuildSSAOBlurConstantBuffer();	// 构建SSAO模糊常量缓冲视图

	void SaveSSAOToBuffer();	// 保存SSAO到帧缓冲（渲染留存）

	void BuildDepthBuffer();	// 构建深度缓冲

	UINT GetAmbientSRVOffset() const;	// 获取环境光SRV偏移
	UINT GetAmbientRTVOffset() const;	// 获取环境光RTV偏移

	UINT GetDepthBufferSRVOffset() const;	// 获取深度缓冲SRV偏移

	UINT GetNoiseBufferSRVOffset() const;	// 获取噪声缓冲SRV偏移

	UINT GetNormalBufferSRVOffset() const;	// 获取法线缓冲SRV偏移
	UINT GetNormalBufferRTVOffset() const;	// 获取法线缓冲RTV偏移

	UINT GetBilateralBlurSRVOffset() const;	// 获取双边模糊SRV偏移
	UINT GetBilateralBlurRTVOffset() const;	// 获取双边模糊RTV偏移

protected:
	void DrawResource();	// 绘制资源
	void DrawSSAO(float DeltaTime);	// 绘制SSAO
	void DrawBilateralBlur(float DeltaTime, const UINT DrawTimes);	// 绘制双边模糊

	void DrawBilateralBlurHorizontal(float DeltaTime);		// 绘制水平双边模糊 
	void DrawBilateralBlurVertical(float DeltaTime);		// 绘制垂直双边模糊

	void DrawBlur(float DeltaTime, bool bHorizontal);	// 绘制模糊

	ID3D12Resource* GetDrawResource(bool bHorizontal);
	CD3DX12_GPU_DESCRIPTOR_HANDLE* GetDrawSRVResource(bool bHorizontal);
	CD3DX12_CPU_DESCRIPTOR_HANDLE* GetDrawRTVResource(bool bHorizontal);

	void SetRoot32BitConstants(bool bHorizontal);	// 设置根32位常量

protected:
	FNormalBuffer NormalBuffer;						// 法线缓冲
	FAmbientBuffer AmbientBuffer;					// 环境光缓冲
	FAmbientBuffer BilateralBlur;					// 双边模糊
	FSSAODirectXRootSignature SSAORootSignature;	// SSAO根签名

	FConstantBufferViews SSAOConstantBufferView;		// SSAO常量缓冲视图
	FConstantBufferViews SSAOBlurConstantBufferView;	// SSAO模糊常量缓冲视图

	FRenderLayerManage* RenderLayer;	// 渲染层级

	FGeometryMap* GeometryMap;	// 几何图

	FSampleVolume SampleVolume;	// 采样体积
	FNoiseBuffer NoiseBuffer;	// 噪声缓冲
};

