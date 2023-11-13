#pragma once

#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"


class FShader;

struct FDirectXPipelineState : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXPipelineState();

	// ��������
	void BuildParam();

	// ��������״̬
	void BuildPipelineState(int psoType);

	// ���Ƶ�ʱ�����
	void ResetPSO(int psoType);

	// ����PSO��ֻ�ṩ����͸����ʹ��
	void ResetPSO();

	void ResetGPSDesc();

	// �����벼��
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InputElementDesc, UINT Size);

	// �󶨸�ǩ��
	void BindRootSignature(ID3D12RootSignature* RootSignature);

	// �󶨶�����ɫ����������ɫ��
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

	// ������ȾĿ��
	void SetRenderTarget(int index, D3D12_RENDER_TARGET_BLEND_DESC& renderTargetBlend);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

	void SetFillModle(bool bWireframe);

	void SetRasterizerState(const CD3DX12_RASTERIZER_DESC& rasterizer);
	void SetDepthStencilState(const CD3DX12_DEPTH_STENCIL_DESC& depthStencilDesc);

private:			
	std::unordered_map<UINT, ComPtr<ID3D12PipelineState>>	PSO{};		// ��Ⱦ����״̬����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	GPSDesc;	// ͼ����Ⱦ����״̬���� ����������ɫ����������ɫ������դ��״̬�����״̬�����벼�ֵ�
	EPipelineState PipelineState = EPipelineState::Solid;		// ��Ⱦ���ͣ�Ĭ��ʵ��ģ����Ⱦģʽ
};

