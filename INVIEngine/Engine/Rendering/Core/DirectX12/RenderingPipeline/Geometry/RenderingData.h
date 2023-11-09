#pragma once

#include "Interface/DirectXDeviceInterface.h"


class CMeshComponent;
class GMesh;
class FRenderingResourcesUpdate;

struct FRenderingData : public IDirectXDeviceInterface_Struct
{
public:
	FRenderingData();
	~FRenderingData();

public:
	UINT GetVertexSizeInBytes() const { return VertexSize * VertexTypeSize; };		// ��ȡ���������ֽڴ�С
	UINT GetIndexSizeInBytes() const { return IndexSize * IndexTypeSize; };			// ��ȡ���������ֽڴ�С

public:
	size_t MeshHash;
	int GeometryKey;
	int MeshObjectIndex;

public:
	UINT IndexSize;				// ������������
	UINT VertexSize;			// ���������

	UINT IndexOffsetPosition;	// ����ƫ��λ��
	UINT VertexOffsetPosition;	// ����ƫ��λ��

	UINT VertexTypeSize;		// �����������͵��ֽڴ�С
	UINT IndexTypeSize;			// �����������͵��ֽڴ�С
	
	DXGI_FORMAT IndexFormat;	// �����������ݸ�ʽ���޷���16��

	XMFLOAT4X4 MaterialTransformationMatrix;			// ���ʱ仯����

	// std::shared_ptr<FRenderingResourcesUpdate> ObjectConstants;		// ��������ָ���ϴ��ѣ�

	XMFLOAT4X4 TextureTransformationMatrix;		// ����仯����

	CMeshComponent* Mesh;
};

