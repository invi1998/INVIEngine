#pragma once

#include "MeshType.h"
#include "Core/Engine.h"
#include "Rendering/Core/Rendering.h"
#include "Shader/Core/Shader.h"

struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 World;
};

class FMesh : public IRenderingInterface
{
public:
	FMesh();
	~FMesh() override;

	virtual void Init() override;

	virtual void Draw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRendingData* InRenderingData);

	static FMesh* CreateMesh(const FMeshRendingData* InRenderingData);

	// ��ȡ����buff��ͼ
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
	// ��ȡ����index��������ͼ
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();

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

	std::shared_ptr<FRenderingResourcesUpdate> ObjectConstants;	// ��������ָ���ϴ��ѣ�

	FShader VertexShader;	// ������ɫ��
	FShader PixelShader;	// ������ɫ��

	std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDesc;		// �������벼�֣�Input Layout���еĵ���Ԫ�ء��� Direct3D 12 �У����벼����һ�������������Ԫ�ص����ݽṹ����ʾ���㻺�����д洢�Ķ������ݵĸ�ʽ�����з�ʽ��

protected:
	UINT IndexSize;				// ��������

	UINT VertexSizeInBytes;		// ���������ֽڴ�С
	UINT VertexStrideInBytes;	// ���������ֽڿ��(��λ�������ݴ�С��

	UINT IndexSizeInBytes;		// ���������ֽڴ�С
	DXGI_FORMAT IndexFormat;	// �����������ݸ�ʽ���޷���16��

};

