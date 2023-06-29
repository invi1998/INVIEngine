#pragma once

#include "MeshType.h"
#include "Core/Engine.h"
#include "Rendering/Core/Rendering.h"

class FMesh : public IRenderingInterface
{
public:
	FMesh();
	~FMesh() override;

	virtual void Init() override;

	virtual void Draw(float DeltaTime) override;

	virtual void BuildMesh(const FMeshRendingData* InRenderingData);

	static FMesh* CreateMesh(const FMeshRendingData* InRenderingData);

protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;			// CPU ���㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				// CPU ����������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		// GPU ���㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		// GPU ����������

	// ��ʱ���������������ǽ�CPU��buffer���ݸ��Ƶ�GPU
	ComPtr<ID3D12Resource> TempVertexBufferPtr;		// ��ʱ ���㻺����
	ComPtr<ID3D12Resource> TempIndexBufferPtr;		// ��ʱ ����������

};

