#pragma once

#include "EngineMinimal.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Mesh/Core/Mesh.h"

#include "Rendering/Core/Rendering.h"
#include "Rendering/Core/RenderingResourcesUpdate.h"
#include "Shader/Core/Shader.h"

class CMeshManager : public CCoreMinimalObject, public IRenderingInterface, public IDirectXDeviceInterface_Struct
{
public:
	CMeshManager();
	~CMeshManager() override;
	void BeginInit() override;
	void Tick(float DeltaTime) override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

	virtual void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);

	// ��ȡ����buff��ͼ
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	// ��ȡ����index��������ͼ
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();

public:
	CMesh* CreateBoxMesh(
		float InHeight,
		float InWidth,
		float InDepth);

	CMesh* CreateConeMesh(
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMesh* CreateCylinderMesh(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMesh* CreatePlaneMesh(
		float InHeight,
		float InWidth,
		uint32_t InHeightSubdivide,
		uint32_t InWidthSubdivide);

	CMesh* CreateSphereMesh(
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMesh* CreateMesh(const string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;			// CPU ���㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				// CPU ����������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		// GPU ���㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		// GPU ����������

	// ��ʱ���������������ǽ�CPU��buffer���ݸ��Ƶ�GPU
	ComPtr<ID3D12Resource> TempVertexBufferPtr;		// ��ʱ ���㻺����
	ComPtr<ID3D12Resource> TempIndexBufferPtr;		// ��ʱ ����������

	ComPtr<ID3D12RootSignature> RootSignature;		// ��ǩ��
	ComPtr<ID3D12PipelineState> PSO;				// ��Ⱦ����״̬����

	ComPtr<ID3D12DescriptorHeap> CBVHeap;			// CPU ���������������ָ��Ҫ�����ĳ�����������ͼ���ڵ���������λ��

	std::shared_ptr<FRenderingResourcesUpdate> ObjectConstants;		// ��������ָ���ϴ��ѣ�
	std::shared_ptr<FRenderingResourcesUpdate> ViewportConstants;	// �ӿڶ�������ָ���ϴ��ѣ�

	FShader VertexShader;	// ������ɫ��
	FShader PixelShader;	// ������ɫ��

	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		// �������벼�֣�Input Layout���еĵ���Ԫ�ء��� Direct3D 12 �У����벼����һ�������������Ԫ�ص����ݽṹ����ʾ���㻺�����д洢�Ķ������ݵĸ�ʽ�����з�ʽ��

protected:
	UINT IndexSize;				// ��������

	UINT VertexSizeInBytes;		// ���������ֽڴ�С
	UINT VertexStrideInBytes;	// ���������ֽڿ��(��λ�������ݴ�С��

	UINT IndexSizeInBytes;		// ���������ֽڴ�С
	DXGI_FORMAT IndexFormat;	// �����������ݸ�ʽ���޷���16��

	XMFLOAT4X4 WorldMatrix;			// �������


};

