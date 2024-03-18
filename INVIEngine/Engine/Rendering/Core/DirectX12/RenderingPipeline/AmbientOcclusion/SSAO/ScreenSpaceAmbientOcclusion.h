#pragma once
#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/ConstantBuffer/ConstantBufferViews.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/AmbientBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/NormalBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RootSignature/SSAODirectXRootSignature.h"

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

	void SaveSSAOToBuffer();	// 保存SSAO到帧缓冲（渲染留存）

protected:
	FNormalBuffer NormalBuffer;						// 法线缓冲
	FAmbientBuffer AmbientBuffer;					// 环境光缓冲
	FSSAODirectXRootSignature SSAORootSignature;	// SSAO根签名

	FConstantBufferViews SSAOConstantBufferView;		// SSAO常量缓冲视图

	FRenderLayerManage* RenderLayer;	// 渲染层级
};

