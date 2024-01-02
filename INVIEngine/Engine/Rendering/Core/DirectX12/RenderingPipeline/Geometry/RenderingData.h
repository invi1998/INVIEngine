#pragma once

#include "Interface/DirectXDeviceInterface.h"


struct FMeshRenderingData;
class CMeshComponent;
class GMesh;
class FRenderingResourcesUpdate;

struct FRenderingData :
	public IDirectXDeviceInterface_Struct,
	public std::enable_shared_from_this<FRenderingData>		// 在当前类中保存一个指向自身的shared_ptr，以便智能指针暴露成裸指针后，可以通过shared_from_this()获取智能指针
{
public:
	FRenderingData();
	~FRenderingData();

public:
	UINT GetVertexSizeInBytes() const { return VertexSize * VertexTypeSize; };		// 获取顶点数据字节大小
	UINT GetIndexSizeInBytes() const { return IndexSize * IndexTypeSize; };			// 获取顶点数据字节大小

public:
	size_t MeshHash;
	int GeometryKey;
	int MeshObjectIndex;

public:
	UINT IndexSize;				// 顶点索引数量
	UINT VertexSize;			// 顶点的数量

	UINT IndexOffsetPosition;	// 索引偏移位置
	UINT VertexOffsetPosition;	// 顶点偏移位置

	UINT VertexTypeSize;		// 顶点数据类型的字节大小
	UINT IndexTypeSize;			// 索引数据类型的字节大小
	
	DXGI_FORMAT IndexFormat;	// 顶点索引数据格式（无符号16）

	XMFLOAT4X4 WorldMatrix;			// 世界矩阵

	// std::shared_ptr<FRenderingResourcesUpdate> ObjectConstants;		// 对象常量（指向上传堆）

	XMFLOAT4X4 TextureTransformationMatrix;		// 纹理变化矩阵

	CMeshComponent* Mesh;

	FMeshRenderingData* MeshRenderingData;

	// AABB包围盒
	BoundingBox Bounds;
};

