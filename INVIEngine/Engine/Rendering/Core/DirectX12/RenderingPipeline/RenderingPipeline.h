#pragma once

#include "EngineMinimal.h"
#include "GeometryMap.h"
#include "Interface/DirectXDeviceInterface.h"

// ��Ⱦ��ˮ��
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:

protected:
	FGeometryMap GeometryMap;

};

