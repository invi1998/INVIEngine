#include "EngineMinimal.h"
#include "ScreenSpaceAmbientOcclusion.h"

#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/NormalBuffer.h"

FScreenSpaceAmbientOcclusion::FScreenSpaceAmbientOcclusion()
{
	NormalBuffer = new FNormalBuffer();
}

FScreenSpaceAmbientOcclusion::~FScreenSpaceAmbientOcclusion()
{
}

void FScreenSpaceAmbientOcclusion::Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState,
                                        FRenderLayerManage* inRenderLayer)
{
	NormalBuffer.Init(inGeometryMap, inDirectXPipelineState, inRenderLayer);
}

void FScreenSpaceAmbientOcclusion::SetBufferSize(int wid, int hei)
{
	NormalBuffer.SetBufferSize(wid, hei);
}

void FScreenSpaceAmbientOcclusion::Draw(float DeltaTime)
{
	NormalBuffer.Draw(DeltaTime);
}

void FScreenSpaceAmbientOcclusion::UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info)
{
	NormalBuffer.UpdateCalculations(DeltaTime, viewport_info);
}

void FScreenSpaceAmbientOcclusion::BuildDescriptor()
{
	NormalBuffer.BuildDescriptor();
	NormalBuffer.BuildRenderTargetRTV();
	NormalBuffer.BuildSRVDescriptor();
	NormalBuffer.BuildRTVDescriptor();
}
