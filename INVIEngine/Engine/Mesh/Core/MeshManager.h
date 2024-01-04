#pragma once

#include "Core/Viewport/ViewportInfo.h"
#include "Interface/DirectXDeviceInterface.h"
#include "Mesh/Core/Mesh.h"

#include "Rendering/Core/Rendering.h"
#include "Rendering/Core/DirectX12/RenderingPipeline/RenderingPipeline.h"

class FRenderingResourcesUpdate;
class CMeshComponent;

class CMeshManager : public CCoreMinimalObject, public IRenderingInterface, public IDirectXDeviceInterface
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
	CMeshComponent* CreateBoxMeshComponent(
		const FCreateObjectParams& InParams,
		float InHeight,
		float InWidth,
		float InDepth);

	CMeshComponent* CreateConeMeshComponent(
		const FCreateObjectParams& InParams,
		float InRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateCylinderMeshComponent(
		const FCreateObjectParams& InParams,
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreatePlaneMeshComponent(
		const FCreateObjectParams& InParams,
		float InHeight,
		float InWidth,
		uint32_t InHeightSubdivide,
		uint32_t InWidthSubdivide);

	CMeshComponent* CreateSphereMeshComponent(
		const FCreateObjectParams& InParams,
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision,
		bool bReverse = false);

	CMeshComponent* CreatePyramidMeshComponent(
		const FCreateObjectParams& InParams,
		EPyramidNumberSides InPyramidNumberSidesType, 
		uint32_t InHeightSubdivide,
		uint32_t InSize);

	CMeshComponent* CreatePipeMeshComponent(
		const FCreateObjectParams& InParams,
		float InTopRadius, 
		float InBottomRadius, 
		float InHeight, 
		float InThickness,
		uint32_t InAxialSubdivision, 
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateTorusMeshComponent(
		const FCreateObjectParams& InParams,
		float InRadius, 
		float InSectionRadius, 
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateCustomMeshComponent(const FCreateObjectParams& InParams, const string& InPath);

	FRenderingPipeline* GetRenderingPipeline() const { return const_cast<FRenderingPipeline*>(&RenderingPipeline); };		// 渲染管线对象

protected:
	FRenderingPipeline RenderingPipeline;		// 渲染管线对象
	
};

