#pragma once
#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"


class CMesh;
class FRenderingResourcesUpdate;

struct FRenderingData : public IDirectXDeviceInterface_Struct
{
public:
	UINT IndexSize;				// ��������

	UINT VertexSizeInBytes;		// ���������ֽڴ�С
	UINT VertexStrideInBytes;	// ���������ֽڿ��(��λ�������ݴ�С��

	UINT IndexSizeInBytes;		// ���������ֽڴ�С
	DXGI_FORMAT IndexFormat;	// �����������ݸ�ʽ���޷���16��

	XMFLOAT4X4 WorldMatrix;			// �������

	std::shared_ptr<FRenderingResourcesUpdate> ObjectConstants;		// ��������ָ���ϴ��ѣ�

	CMesh* Mesh;
};

