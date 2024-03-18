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

	void Build();	// ����AO����
	void BuildPSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& OutPSODesc);	// ����PSO

	void BindBuildPso();	// �󶨹���PSO

	void Draw(float DeltaTime);
	void DrawSSAOConstantBuffer(float DeltaTime, const FViewportInfo& viewport_info);	// ����SSAO����buffer
	void UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info);

	void BuildDescriptor();		// ���������� ���ڰ󶨵������� RenderTargetView ShaderResourceView

	void BuildSSAOConstantBufferView();	// ����SSAO����������ͼ

	void SaveSSAOToBuffer();	// ����SSAO��֡���壨��Ⱦ���棩

protected:
	FNormalBuffer NormalBuffer;						// ���߻���
	FAmbientBuffer AmbientBuffer;					// �����⻺��
	FSSAODirectXRootSignature SSAORootSignature;	// SSAO��ǩ��

	FConstantBufferViews SSAOConstantBufferView;		// SSAO����������ͼ

	FRenderLayerManage* RenderLayer;	// ��Ⱦ�㼶
};

