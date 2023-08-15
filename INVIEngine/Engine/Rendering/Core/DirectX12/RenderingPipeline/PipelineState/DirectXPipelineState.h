#pragma once
#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"


class FShader;

struct FDirectXPipelineState : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXPipelineState();

	void BuildPipeline();

	void ResetGPSDesc();

	// �����벼��
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InputElementDesc, UINT Size);

	// �󶨸�ǩ��
	void BindRootSignature(ID3D12RootSignature* RootSignature);

	// �󶨶�����ɫ����������ɫ��
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);


private:
	ComPtr<ID3D12PipelineState> PSO;				// ��Ⱦ����״̬����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	GPSDesc;	// ͼ����Ⱦ����״̬���� ����������ɫ����������ɫ������դ��״̬�����״̬�����벼�ֵ�
};

