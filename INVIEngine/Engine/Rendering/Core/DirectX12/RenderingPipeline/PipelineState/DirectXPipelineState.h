#pragma once

#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"


class FShader;

struct FDirectXPipelineState : public IDirectXDeviceInterface_Struct
{
public:
	FDirectXPipelineState();

	// 构建参数
	void BuildParam();

	// 构建管线状态
	void BuildPipelineState(int psoType);

	// 绘制的时候调用
	void ResetPSO(int psoType);

	// 重置PSO，只提供给不透明层使用
	void ResetPSO();

	void ResetGPSDesc();

	// 绑定输入布局
	void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InputElementDesc, UINT Size);

	// 绑定根签名
	void BindRootSignature(ID3D12RootSignature* RootSignature);

	// 绑定顶点着色器和像素着色器
	void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

	// 设置渲染目标
	void SetRenderTarget(int index, D3D12_RENDER_TARGET_BLEND_DESC& renderTargetBlend);

	void PreDraw(float DeltaTime);
	void Draw(float DeltaTime);
	void PostDraw(float DeltaTime);

	void SetFillModle(bool bWireframe);

	void SetRasterizerState(const CD3DX12_RASTERIZER_DESC& rasterizer);
	void SetDepthStencilState(const CD3DX12_DEPTH_STENCIL_DESC& depthStencilDesc);

	// 将当前的渲染管线状态对象设置为默认的渲染管线状态对象
	void SaveGPSDescAsDefault();

public:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC& GetGPSDesc() { return GPSDesc; }

private:
	std::unordered_map<UINT, ComPtr<ID3D12PipelineState>>	PSO{};		// 渲染管线状态对象
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	GPSDesc{};	// 图形渲染管线状态描述 包括顶点着色器、像素着色器、光栅化状态、混合状态、输入布局等
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	DefaultGPSDesc{};	// 默认图形渲染管线状态描述 包括顶点着色器、像素着色器、光栅化状态、混合状态、输入布局等
	EPipelineState PipelineState = EPipelineState::Solid;		// 渲染类型，默认实体模型渲染模式
};

