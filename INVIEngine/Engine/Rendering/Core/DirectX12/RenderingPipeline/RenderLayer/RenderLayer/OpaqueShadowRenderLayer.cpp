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
	DirectXPipelineState->ResetPSO();

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

	VertexShader.BuildShader(L"Shader/HLSL/Shadow.hlsl", "VSMain", "vs_5_1", D3dShaderMacro.data());
	PixelShader.BuildShader(L"Shader/HLSL/Shadow.hlsl", "PSMain", "ps_5_1", D3dShaderMacro.data());
	// ��shader
	DirectXPipelineState->BindShader(VertexShader, PixelShader);

	// ���벼��
	InputElementDesc =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}	// uv���꣬�������ƫ����52�ֽڣ���Ϊ��������λ����3��Ԫ�أ�ÿ��Ԫ����4�ֽڣ�32λ��������uv����͵�ƫ�� ��3��*4 = 12�ֽڵ�ƫ����

	};

	// �����벼��
	DirectXPipelineState->BindInputLayout(InputElementDesc.data(), InputElementDesc.size());
}

void FOpaqueShadowRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC& GPSDesc = DirectXPipelineState->GetGPSDesc();

	// ƫ�Ʋ��� Ϊ�˽����Ӱʧ������
	// d = 1 / pow(2, 24);  2^24
	// b��ƫ������ = DepthBias���̶�ƫ������ * d + SlopeScaledDepthBias���������ӣ� * MaxDepthSlope�������ȣ�
	GPSDesc.RasterizerState.DepthBias = 100000;				// б�� �̶�ƫ����
	GPSDesc.RasterizerState.DepthBiasClamp = 0.0f;			// ����	ƫ���������ƣ�ƫ�������ޣ����ƫ����
	GPSDesc.RasterizerState.SlopeScaledDepthBias = 1.0f;	// ƫ���� ���ݶ����б�������Ƶ�һ����������

	// �ر�RenderTarget������ʽ����Ϊλ�ã�Ȼ��renderTarget��������Ϊ0��
	GPSDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	GPSDesc.NumRenderTargets = 0;

	DirectXPipelineState->BuildPipelineState(EPipelineState::OrthographicShadow);		// ����������Ӱpso

	GPSDesc.RasterizerState.DepthBias = 100;				// б�� �̶�ƫ����
	DirectXPipelineState->BuildPipelineState(EPipelineState::PerspectiveShadow);		// ����͸����Ӱpso
	
}

void FOpaqueShadowRenderLayer::ResetPSO()
{
	DirectXPipelineState->ResetPSO(OrthographicShadow);
}

void FOpaqueShadowRenderLayer::ResetPSO(EPipelineState ps)
{
	FRenderLayer::ResetPSO(ps);

	DirectXPipelineState->ResetPSO(ps);
}

void FOpaqueShadowRenderLayer::DrawMesh(float DeltaTime, ERenderCondition rc)
{
	FRenderLayer::DrawMesh(DeltaTime, rc);
}
