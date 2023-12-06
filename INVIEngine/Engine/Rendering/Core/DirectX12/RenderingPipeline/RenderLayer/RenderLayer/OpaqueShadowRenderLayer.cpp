#include "EngineMinimal.h"
#include "OpaqueShadowRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FOpaqueShadowRenderLayer::FOpaqueShadowRenderLayer()
{
	RenderPriority = 1280;
	RenderLayerType = EMeshRenderLayerType::RENDER_LAYER_OPAQUE_SHADOW;
}

FOpaqueShadowRenderLayer::~FOpaqueShadowRenderLayer()
{
}

void FOpaqueShadowRenderLayer::PreDraw(float deltaTime)
{
	FRenderLayer::PreDraw(deltaTime);
}

void FOpaqueShadowRenderLayer::Draw(float deltaTime)
{
	ResetPSO();

	FRenderLayer::Draw(deltaTime);
}

void FOpaqueShadowRenderLayer::PostDraw(float deltaTime)
{
	FRenderLayer::PostDraw(deltaTime);
}

void FOpaqueShadowRenderLayer::BuildShader()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///����shader HLSL
	///

	std::vector<ShaderType::FShaderMacro> ShaderMacro;
	BuildShaderMacro(ShaderMacro);

	std::vector<D3D_SHADER_MACRO> D3dShaderMacro;
	ShaderType::ToD3DShaderMacro(ShaderMacro, D3dShaderMacro);

	VertexShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "VSMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "PSMain", "ps_5_1", D3dShaderMacro.data());
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

void FOpaqueShadowRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC& GPSDesc = DirectXPipelineState->GetGPSDesc();

	// b = DepthBias * d + SlopeScaledDepthBias * MaxDepthSlope
	GPSDesc.RasterizerState.DepthBias = 100000;				// б�� �̶�ƫ����
	GPSDesc.RasterizerState.DepthBiasClamp = 0.0f;			// ����	ƫ���������ƣ�ƫ�������ޣ����ƫ����
	GPSDesc.RasterizerState.SlopeScaledDepthBias = 1.0f;	// ƫ���� ���ݶ����б�����������ŵ�һ������

	DirectXPipelineState->BuildPipelineState(EPipelineState::Shadow);
	
}

void FOpaqueShadowRenderLayer::ResetPSO()
{



	DirectXPipelineState->ResetPSO(Shadow);
}

void FOpaqueShadowRenderLayer::DrawMesh(float DeltaTime)
{
	FRenderLayer::DrawMesh(DeltaTime);
}
