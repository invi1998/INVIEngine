#include "EngineMinimal.h"
#include "OperationHandleRotPlaneRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FOperationHandleRotPlaneRenderLayer::FOperationHandleRotPlaneRenderLayer()
{
	RenderPriority = 5678;

	RenderLayerType = EMeshRenderLayerType::RENDER_LAYER_ROT_PLANE;
}

void FOperationHandleRotPlaneRenderLayer::BuildShader()
{
	std::vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	std::vector<D3D_SHADER_MACRO> D3dShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3dShaderMacro);

	VertexShader.BuildShader(L"Shader/HLSL/OperationHandleRotPlane.hlsl", "VSMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/OperationHandleRotPlane.hlsl", "PSMain", "ps_5_1", D3dShaderMacro.data());
	// ��shader
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	// ���벼��
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	// ��ɫ���������ƫ����12�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ�������Ե�����ɫ������� 3*4 = 12�ֽڵ�ƫ����
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},	 // ���ߣ��������ƫ����28�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ��, ������ɫ4��Ԫ�أ����Ե��˷�������͵�ƫ�� ��3+4��*4 = 28�ֽڵ�ƫ����
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}, // ���ߣ�U���ߣ����������ƫ����32�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ��, ������ɫ4��Ԫ�أ��ټ���3������Ԫ�أ�������������͵�ƫ�� ��3+4+3��*4 = 40�ֽڵ�ƫ����
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}	// uv���꣬�������ƫ����52�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ��, ������ɫ4��Ԫ�أ��ټ���3������Ԫ�أ��ټ���3������Ԫ�أ�����uv����͵�ƫ�� ��3+4+3+3��*4 = 52�ֽڵ�ƫ����

	};

	// �����벼��
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FOperationHandleRotPlaneRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	CD3DX12_RASTERIZER_DESC RasterizerDesc(D3D12_DEFAULT);
	RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;		// ���ģʽΪʵ��
	RasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;			// ���ü���Ҳ���Ǳ���Ҳ��Ⱦ

	DirectXPipelineState->SetRasterizerState(RasterizerDesc);

	CD3DX12_DEPTH_STENCIL_DESC DepthStencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;	// ��Ȳ��Ժ�������������Ϊʼ��ͨ����Ȳ��ԣ������Ͳ�����Ϊ��Ȳ��Զ���ʧһЩƬ�Σ���Ϊ�����������ģ���ϵģ����Կ��ܻ���һЩƬ�α���Ȳ��Ը������ˣ�������������Ϊʼ��ͨ����Ȳ��ԣ�
	DirectXPipelineState->SetDepthStencilState(DepthStencilDesc);

	DirectXPipelineState->BuildPipelineState(EPipelineState::RotatePlane);	// ����PSO
}

void FOperationHandleRotPlaneRenderLayer::Draw(float deltaTime)
{
	ResetPSO();

	FRenderLayer::Draw(deltaTime);
}

void FOperationHandleRotPlaneRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(EPipelineState::RotatePlane);	// ����PSO
}
