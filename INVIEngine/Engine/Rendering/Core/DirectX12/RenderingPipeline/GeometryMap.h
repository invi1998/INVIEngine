#pragma once

#include "EngineMinimal.h"
#include "RenderingData.h"
#include "Interface/DirectXDeviceInterface.h"

struct FGeometry : IDirectXDeviceInterface_Struct
{
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;			// CPU ���㻺����
	ComPtr<ID3DBlob> CPUIndexBufferPtr;				// CPU ����������

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;		// GPU ���㻺����
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;		// GPU ����������

	// ��ʱ���������������ǽ�CPU��buffer���ݸ��Ƶ�GPU
	ComPtr<ID3D12Resource> TempVertexBufferPtr;		// ��ʱ ���㻺����
	ComPtr<ID3D12Resource> TempIndexBufferPtr;		// ��ʱ ����������

	std::vector<FRenderingData> DescribeMeshRenderingData;					// ��Ⱦ����
};

// ����ģ��ӳ��׶�
struct FGeometryMap : IDirectXDeviceInterface_Struct
{
	map<int, FGeometry> Geometries;
};

