#pragma once
#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"


class GMesh;
class FRenderingResourcesUpdate;

struct FRenderingData : public IDirectXDeviceInterface_Struct
{
public:
	FRenderingData();

public:
	UINT GetVertexSizeInBytes() const { return VertexSize * VertexTypeSize; };		// ��ȡ���������ֽڴ�С
	UINT GetIndexSizeInBytes() const { return IndexSize * IndexTypeSize; };			// ��ȡ���������ֽڴ�С

public:
	UINT IndexSize;				// ������������
	UINT VertexSize;			// ���������

	UINT IndexOffsetPosition;	// ����ƫ��λ��
	UINT VertexOffsetPosition;	// ����ƫ��λ��

	UINT VertexTypeSize;		// �����������͵��ֽڴ�С
	UINT IndexTypeSize;			// �����������͵��ֽڴ�С
	
	DXGI_FORMAT IndexFormat;	// �����������ݸ�ʽ���޷���16��

	XMFLOAT4X4 WorldMatrix;			// �������

	std::shared_ptr<FRenderingResourcesUpdate> ObjectConstants;		// ��������ָ���ϴ��ѣ�

	GMesh* Mesh;
};

