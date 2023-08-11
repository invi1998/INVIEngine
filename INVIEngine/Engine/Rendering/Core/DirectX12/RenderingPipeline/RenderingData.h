#pragma once
#include "EngineMinimal.h"
#include "Interface/DirectXDeviceInterface.h"


class CMesh;
class FRenderingResourcesUpdate;

struct FRenderingData : public IDirectXDeviceInterface_Struct
{
public:
	UINT IndexSize;				// 顶点数量

	UINT VertexSizeInBytes;		// 顶点数据字节大小
	UINT VertexStrideInBytes;	// 顶点数据字节跨度(单位顶点数据大小）

	UINT IndexSizeInBytes;		// 顶点数据字节大小
	DXGI_FORMAT IndexFormat;	// 顶点索引数据格式（无符号16）

	XMFLOAT4X4 WorldMatrix;			// 世界矩阵

	std::shared_ptr<FRenderingResourcesUpdate> ObjectConstants;		// 对象常量（指向上传堆）

	CMesh* Mesh;
};

