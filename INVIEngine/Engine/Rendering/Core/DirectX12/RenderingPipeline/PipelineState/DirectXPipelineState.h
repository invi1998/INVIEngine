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

	// 绑定输入布局
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InputElementDesc, UINT Size);

	// 绑定根签名
	void BindRootSignature(ID3D12RootSignature* RootSignature);

	// 绑定顶点着色器和像素着色器
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);


private:
	ComPtr<ID3D12PipelineState> PSO;				// 渲染管线状态对象
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	GPSDesc;	// 图形渲染管线状态描述 包括顶点着色器、像素着色器、光栅化状态、混合状态、输入布局等
};

