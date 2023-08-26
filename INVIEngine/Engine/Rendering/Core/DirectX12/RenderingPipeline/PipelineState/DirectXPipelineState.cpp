#include "DirectXPipelineState.h"

#include "Shader/Core/Shader.h"
#include "Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"

FDirectXPipelineState::FDirectXPipelineState()
{
	PSO.insert(pair<UINT, ComPtr<ID3D12PipelineState>>(4, ComPtr<ID3D12PipelineState>()));		// 4 表示 线框渲染模式的渲染管线
	PSO.insert(pair<UINT, ComPtr<ID3D12PipelineState>>(5, ComPtr<ID3D12PipelineState>()));		// 5 表示 面渲染模式的渲染管线
}

void FDirectXPipelineState::BuildPipeline()
{
	// 配置光栅化状态
	GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);		// 光栅化状态

	GPSDesc.SampleMask = UINT_MAX;		// 多重采样遮罩（混合状态下的实例蒙版）多重采样下，最多是可以采32个的样板（000...000),每个0表示一个样板位，有32个，你想采哪个就设置哪位为1，UINT_MAX表示0xffffffff采样全部的样板

	// 指定图元拓扑类型
	GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;		// 三角形

	// 指定渲染目标
	GPSDesc.NumRenderTargets = 1;

	// 指定混合状态
	GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);		// 默认混合

	// 启用深度模板
	GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	// 默认深度模板

	// 设置采样
	GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetDXGISampleCount();		// 采样数量
	GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetDXGISampleQuality();	// 采样质量
	GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();			// 渲染目标视图格式（后台缓冲区格式）
	GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();			// 深度模板缓冲区格式

	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;			// 以线框方式显示

	// 通过D3D创建渲染管线状态对象
	ANALYSIS_RESULT(GetD3dDevice()->CreateGraphicsPipelineState(
		&GPSDesc,
		IID_PPV_ARGS(&PSO[EPipelineState::Wireframe])
	));

	GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;			// 以实体方式显示

	// 通过D3D创建渲染管线状态对象
	ANALYSIS_RESULT(GetD3dDevice()->CreateGraphicsPipelineState(
		&GPSDesc,
		IID_PPV_ARGS(&PSO[EPipelineState::Solid])
	));
}

void FDirectXPipelineState::ResetGPSDesc()
{
	memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
}

void FDirectXPipelineState::BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InputElementDesc, UINT Size)
{
	// 绑定输入布局
	GPSDesc.InputLayout.pInputElementDescs = InputElementDesc;
	GPSDesc.InputLayout.NumElements = Size;
}

void FDirectXPipelineState::BindRootSignature(ID3D12RootSignature* RootSignature)
{
	// 绑定根签名
	GPSDesc.pRootSignature = RootSignature;
}

void FDirectXPipelineState::BindShader(const FShader& InVertexShader, const FShader& InPixelShader)
{
	// 绑定shader
	GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(InVertexShader.GetBufferPointer());
	GPSDesc.VS.BytecodeLength = InVertexShader.GetBufferSize();

	GPSDesc.PS.pShaderBytecode = InPixelShader.GetBufferPointer();
	GPSDesc.PS.BytecodeLength = InPixelShader.GetBufferSize();
}

void FDirectXPipelineState::PreDraw(float DeltaTime)
{
	// 重置命令列表，因为我们每一帧都会有新的提交列表
	GetD3dGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO[PipelineState].Get());
}

void FDirectXPipelineState::Draw(float DeltaTime)
{
	if (FInput::IsKeyPressed(Key::Num4))
	{
		// 按下4键切换为实体渲染
		PipelineState = EPipelineState::Solid;
	}
	else if (FInput::IsKeyPressed(Key::Num5))
	{
		// 按下5键切换为线框渲染
		PipelineState = EPipelineState::Wireframe;
	}
}

void FDirectXPipelineState::PostDraw(float DeltaTime)
{
}
