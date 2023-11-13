#include "EngineMinimal.h"
#include "BackgroundRenderLayer.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/GeometryMap.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineState/DirectXPipelineState.h"

FBackgroundRenderLayer::FBackgroundRenderLayer()
{
	RenderPriority = 1000;
	RenderLayerType = EMeshRenderLayerType::RENDER_LAYER_BACKGROUND;
}

FBackgroundRenderLayer::~FBackgroundRenderLayer()
{
}

int FBackgroundRenderLayer::GetRenderLayerType() const
{
	return RenderLayerType;
}

void FBackgroundRenderLayer::PreDraw(float deltaTime)
{
	FRenderLayer::PreDraw(deltaTime);
}

void FBackgroundRenderLayer::Draw(float deltaTime)
{
	FRenderLayer::Draw(deltaTime);

	DirectXPipelineState->ResetPSO(RenderLayerType);
}

void FBackgroundRenderLayer::PostDraw(float deltaTime)
{
	FRenderLayer::PostDraw(deltaTime);
}

void FBackgroundRenderLayer::Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState)
{
	FRenderLayer::Init(geometry, directXPipelineState);
}

void FBackgroundRenderLayer::BuildShader()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///����shader HLSL
	///

	char TextureNumBuff[10] = { 0 };

	// shader�궨��
	D3D_SHADER_MACRO ShaderMacro[] = {
		"TEXTURE2DNUM", _itoa(GeometryMap->GetDrawTextureCount(), TextureNumBuff, 10),
		NULL, NULL,
	};

	VertexShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "VSMain", "vs_5_1", ShaderMacro);
	PixelShader.BuildShader(L"Shader/HLSL/V_P_Shader.hlsl", "PSMain", "ps_5_1", ShaderMacro);
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

void FBackgroundRenderLayer::UpdateCaculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	FRenderLayer::UpdateCaculations(DeltaTime, ViewportInfo);
}

void FBackgroundRenderLayer::BuildPSO()
{
	FRenderLayer::BuildPSO();

	DirectXPipelineState->BuildPipelineState(RenderLayerType);

	
}