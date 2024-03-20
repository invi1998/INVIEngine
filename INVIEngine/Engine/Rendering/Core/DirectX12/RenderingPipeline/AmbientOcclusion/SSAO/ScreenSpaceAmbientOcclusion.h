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

	void Build();	// ����AO����
	void BuildPSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC& OutPSODesc);	// ����PSO

	void BindBuildPso();	// �󶨹���PSO

	void Draw(float DeltaTime);
	
	void DrawSSAOConstantBuffer(float DeltaTime, const FViewportInfo& viewport_info);	// ����SSAO����buffer
	void UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info);
	

	void BuildDescriptor();		// ���������� ���ڰ󶨵������� RenderTargetView ShaderResourceView

	void BuildSSAOConstantBufferView();	// ����SSAO����������ͼ

	void BuildSSAOBlurConstantBuffer();	// ����SSAOģ������������ͼ

	void SaveSSAOToBuffer();	// ����SSAO��֡���壨��Ⱦ���棩

	void BuildDepthBuffer();	// ������Ȼ���

	UINT GetAmbientSRVOffset() const;	// ��ȡ������SRVƫ��
	UINT GetAmbientRTVOffset() const;	// ��ȡ������RTVƫ��

	UINT GetDepthBufferSRVOffset() const;	// ��ȡ��Ȼ���SRVƫ��

	UINT GetNoiseBufferSRVOffset() const;	// ��ȡ��������SRVƫ��

	UINT GetNormalBufferSRVOffset() const;	// ��ȡ���߻���SRVƫ��
	UINT GetNormalBufferRTVOffset() const;	// ��ȡ���߻���RTVƫ��

	UINT GetBilateralBlurSRVOffset() const;	// ��ȡ˫��ģ��SRVƫ��
	UINT GetBilateralBlurRTVOffset() const;	// ��ȡ˫��ģ��RTVƫ��

protected:
	void DrawResource();	// ������Դ
	void DrawSSAO(float DeltaTime);	// ����SSAO
	void DrawBilateralBlur(float DeltaTime, const UINT DrawTimes);	// ����˫��ģ��

	void DrawBilateralBlurHorizontal(float DeltaTime);		// ����ˮƽ˫��ģ�� 
	void DrawBilateralBlurVertical(float DeltaTime);		// ���ƴ�ֱ˫��ģ��

	void DrawBlur(float DeltaTime, bool bHorizontal);	// ����ģ��

	ID3D12Resource* GetDrawResource(bool bHorizontal);
	CD3DX12_GPU_DESCRIPTOR_HANDLE* GetDrawSRVResource(bool bHorizontal);
	CD3DX12_CPU_DESCRIPTOR_HANDLE* GetDrawRTVResource(bool bHorizontal);

	void SetRoot32BitConstants(bool bHorizontal);	// ���ø�32λ����

protected:
	FNormalBuffer NormalBuffer;						// ���߻���
	FAmbientBuffer AmbientBuffer;					// �����⻺��
	FAmbientBuffer BilateralBlur;					// ˫��ģ��
	FSSAODirectXRootSignature SSAORootSignature;	// SSAO��ǩ��

	FConstantBufferViews SSAOConstantBufferView;		// SSAO����������ͼ
	FConstantBufferViews SSAOBlurConstantBufferView;	// SSAOģ������������ͼ

	FRenderLayerManage* RenderLayer;	// ��Ⱦ�㼶

	FGeometryMap* GeometryMap;	// ����ͼ

	FSampleVolume SampleVolume;	// �������
	FNoiseBuffer NoiseBuffer;	// ��������
};

