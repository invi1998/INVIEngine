#pragma once
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/PipelineType.h"
#include "Shader/Core/Shader.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/Geometry/RenderingData.h"
#include "Shader/Core/ShaderType.h"

struct FViewportInfo;
struct FDirectXPipelineState;
struct FGeometryMap;

class FRenderLayer : public IDirectXDeviceInterface, public std::enable_shared_from_this<FRenderLayer>
{
	friend struct FGeometry;
	friend struct FGeometryMap;
public:
	FRenderLayer();
	virtual ~FRenderLayer() = default;

	void RegisterRenderLayer();

	const UINT GetRenderPriority() const { return RenderPriority; }

	virtual void PreDraw(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);

	virtual void BuildShaderMacro(std::vector<ShaderType::FShaderMacro>& inShaderMacro);

public:
	virtual void Init(FGeometryMap* geometry, FDirectXPipelineState* directXPipelineState);

	virtual void BuildShader() {}

	virtual void DrawObject(float deltaTime, std::weak_ptr<FRenderingData>& weakRenderDate, ERenderCondition rc=RC_Always);
	virtual void FindObjectDraw(float delta_time, const CMeshComponent* key);

	virtual int GetRenderLayerType() const { return RenderLayerType; };

	virtual void UpdateCaculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	virtual void BuildPSO();

	// �������ù���״̬���� PSO
	virtual void ResetPSO();

	virtual void ResetPSO(EPipelineState ps);

	// ��Ⱦ�����������ù���״̬���� PSO
	virtual void DrawMesh(float DeltaTime, ERenderCondition rc = RC_Always);

protected:
	UINT RenderPriority;
	EMeshRenderLayerType RenderLayerType;

	FShader VertexShader;
	FShader PixelShader;

	// �� Direct3D 12 �У����벼����һ�������������Ԫ�ص����ݽṹ����ʾ���㻺�����д洢�Ķ������ݵĸ�ʽ�����з�ʽ��
	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc{};		// �������벼�֣�Input Layout���еĵ���Ԫ�ء�

	std::vector<std::weak_ptr<FRenderingData>> RenderData{};

	FGeometryMap* GeometryMap{};
	FDirectXPipelineState* DirectXPipelineState{};	// ����״̬ pso
};

