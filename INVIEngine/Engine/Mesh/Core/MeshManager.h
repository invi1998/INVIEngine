#pragma once

#include "EngineMinimal.h"
#include "Core/Viewport/ViewportInfo.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Mesh/Core/Mesh.h"

#include "Rendering/Core/Rendering.h"
#include "Rendering/Core/RenderingResourcesUpdate.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderingPipeline.h"
#include "Shader/Core/Shader.h"

class FRenderingResourcesUpdate;

class CMeshManager : public CCoreMinimalObject, public IRenderingInterface, public IDirectXDeviceInterface_Struct
{
public:
	CMeshManager();
	~CMeshManager() override;
	void BeginInit() override;
	void Tick(float DeltaTime) override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;

	virtual void BuildMesh();

	virtual void UpdateCalculations(float delta_time, const FViewportInfo& viewport_info);

public:
	GMesh* CreateBoxMesh(
		float InHeight,
		float InWidth,
		float InDepth);

	GMesh* CreateConeMesh(
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	GMesh* CreateCylinderMesh(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	GMesh* CreatePlaneMesh(
		float InHeight,
		float InWidth,
		uint32_t InHeightSubdivide,
		uint32_t InWidthSubdivide);

	GMesh* CreateSphereMesh(
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	GMesh* CreateMesh(const string& InPath);

protected:
	FRenderingPipeline RenderingPipeline;		// 渲染管线对象

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);
	
};

