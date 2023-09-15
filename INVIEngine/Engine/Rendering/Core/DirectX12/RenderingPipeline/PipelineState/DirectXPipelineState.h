#pragma once
#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"


class FShader;

struct FDirectXPipelineState : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXPipelineState();

	void BuildPipelineState();

	void ResetGPSDesc();

	// �����벼��
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InputElementDesc, UINT Size);

	// �󶨸�ǩ��
	void BindRootSignature(ID3D12RootSignature* RootSignature);

	// �󶨶�����ɫ����������ɫ��
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);


private:			
	std::unordered_map<UINT, ComPtr<ID3D12PipelineState>>	PSO;		// ��Ⱦ����״̬����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	GPSDesc;	// ͼ����Ⱦ����״̬���� ����������ɫ����������ɫ������դ��״̬�����״̬�����벼�ֵ�
	EPipelineState PipelineState = EPipelineState::Solid;		// ��Ⱦ���ͣ�Ĭ��ʵ��ģ����Ⱦģʽ
};

