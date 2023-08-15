#pragma once

#include "EngineMinimal.h"
#include "GeometryMap.h"
#include "Interface/DirectXDeviceInterface.h"

// äÖÈ¾Á÷Ë®Ïß
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
	FRenderingPipeline();

	void BuildMesh(CMesh* Mesh, const FMeshRenderingData& MeshData);

protected:
	FGeometryMap GeometryMap;

};

