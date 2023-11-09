#include "EngineMinimal.h"

#include "MeshManager.h"

#include "ObjectTransformation.h"
#include "Component/Mesh/BoxMeshComponent.h"
#include "Component/Mesh/ConeMeshComponent.h"
#include "Component/Mesh/CustomMeshComponent.h"
#include "Component/Mesh/CylinderMeshComponent.h"
#include "Component/Mesh/PipeMeshComponent.h"
#include "Component/Mesh/PlaneMeshComponent.h"
#include "Component/Mesh/PyramidMeshComponent.h"
#include "Component/Mesh/SphereMeshComponent.h"
#include "Component/Mesh/TorusMeshComponent.h"
#include "Config/EngineRenderConfig.h"
#include "Core/Viewport/ViewportTransformation.h"
#include "Mesh/BoxMesh.h"
#include "Mesh/CylinderMesh.h"
#include "Mesh/ConeMesh.h"
#include "Mesh/PlaneMesh.h"
#include "Mesh/CustomMesh.h"
#include "Mesh/SphereMesh.h"
#include "Rendering/Core/Buffer/ConstructBuffer.h"
#include "Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "Component/Mesh/Core/MeshComponent.h"
#include "Core/Construction/MeshConstruction.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
= default;

void CMeshManager::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void CMeshManager::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);
}

void CMeshManager::Init()
{
	IRenderingInterface::Init();
}

void CMeshManager::PreDraw(float DeltaTime)
{
	IRenderingInterface::PreDraw(DeltaTime);

	RenderingPipeline.PreDraw(DeltaTime);
}

void CMeshManager::Draw(float DeltaTime)
{
	IRenderingInterface::Draw(DeltaTime);

	RenderingPipeline.Draw(DeltaTime);
}

void CMeshManager::PostDraw(float DeltaTime)
{
	IRenderingInterface::PostDraw(DeltaTime);

	RenderingPipeline.PostDraw(DeltaTime);
}

void CMeshManager::BuildMesh()
{
	RenderingPipeline.BuildPipeline();
}

void CMeshManager::UpdateCalculations(float delta_time, const FViewportInfo& viewport_info)
{
	RenderingPipeline.UpdateCalculations(delta_time, viewport_info);
}

CMeshComponent* CMeshManager::CreateBoxMeshComponent(float InHeight, float InWidth, float InDepth)
{
	return MeshConstruction::CreateMeshComponent<CBoxMeshComponent>(this, InHeight, InWidth, InDepth);
}

CMeshComponent* CMeshManager::CreateConeMeshComponent(float InRadius, float InHeight, uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	return MeshConstruction::CreateMeshComponent<CConeMeshComponent>(this, InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManager::CreateCylinderMeshComponent(float InTopRadius, float InBottomRadius, float InHeight,
	uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return MeshConstruction::CreateMeshComponent<CCylinderMeshComponent>(this, InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManager::CreatePlaneMeshComponent(float InHeight, float InWidth, uint32_t InHeightSubdivide,
	uint32_t InWidthSubdivide)
{
	return MeshConstruction::CreateMeshComponent<CPlaneMeshComponent>(this, InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

CMeshComponent* CMeshManager::CreateSphereMeshComponent(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return MeshConstruction::CreateMeshComponent<CSphereMeshComponent>(this, InRadius, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManager::CreatePyramidMeshComponent(EPyramidNumberSides InPyramidNumberSidesType, uint32_t InHeightSubdivide, uint32_t InSize)
{
	return MeshConstruction::CreateMeshComponent<CPyramidMeshComponent>(this, InPyramidNumberSidesType, InHeightSubdivide, InSize);
}

CMeshComponent* CMeshManager::CreatePipeMeshComponent(float InTopRadius, float InBottomRadius, float InHeight, float InThickness, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return MeshConstruction::CreateMeshComponent<CPipeMeshComponent>(this, InTopRadius, InBottomRadius, InHeight, InThickness, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManager::CreateTorusMeshComponent(float InRadius, float InSectionRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return MeshConstruction::CreateMeshComponent<CTorusMeshComponent>(this, InRadius, InSectionRadius, InAxialSubdivision, InHeightSubdivision);
}

CMeshComponent* CMeshManager::CreateCustomMeshComponent(const string& InPath)
{
	return MeshConstruction::CreateMeshComponent<CCustomMeshComponent>(this, InPath);
}


