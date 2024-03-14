#pragma once
#include "Interface/DirectXDeviceInterface.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/AmbientBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderBuffer/NormalBuffer.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RootSignature/SSAODirectXRootSignature.h"

struct FViewportInfo;
struct FDirectXPipelineState;
struct FGeometryMap;

struct FScreenSpaceAmbientOcclusion : public IDirectXDeviceInterface_Struct
{
public:
	FScreenSpaceAmbientOcclusion();
	~FScreenSpaceAmbientOcclusion();

	void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState, FRenderLayerManage* inRenderLayer);
	void SetBufferSize(int wid, int hei);

	void Draw(float DeltaTime);
	void UpdateCalculations(float DeltaTime, const FViewportInfo& viewport_info);

	void BuildDescriptor();		// 构建描述符 用于绑定到管线上 RenderTargetView ShaderResourceView

protected:
	FNormalBuffer NormalBuffer;						// 法线缓冲
	FAmbientBuffer AmbientBuffer;					// 环境光缓冲
	FSSAODirectXRootSignature SSAORootSignature;	// SSAO根签名
};

