#include "MeshManager.h"

#include "ObjectTransformation.h"
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

GMesh* CMeshManager::CreateBoxMesh(float InHeight, float InWidth, float InDepth)
{
	return CreateMesh<GBoxMesh>(InHeight, InWidth, InDepth);
}

GMesh* CMeshManager::CreateConeMesh(float InRadius, float InHeight, uint32_t InAxialSubdivision,
	uint32_t InHeightSubdivision)
{
	return CreateMesh<GConeMesh>(InRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

GMesh* CMeshManager::CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight,
	uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<GCylinderMesh>(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

GMesh* CMeshManager::CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide,
	uint32_t InWidthSubdivide)
{
	return CreateMesh<GPlaneMesh>(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

GMesh* CMeshManager::CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
	return CreateMesh<GSphereMesh>(InRadius, InAxialSubdivision, InHeightSubdivision);
}

GMesh* CMeshManager::CreateMesh(const string& InPath)
{
	return CreateMesh<GCustomMesh>(InPath);
}

template <typename T, typename ... ParamTypes>
T* CMeshManager::CreateMesh(ParamTypes&&... Params)
{
	T* MyMesh = new T();

	// 提取模型资源
	FMeshRenderingData MeshData;
	MyMesh->CreateMesh(MeshData, std::forward<ParamTypes>(Params)...);

	MyMesh->BeginInit();

	// 构建mesh
	// BuildMesh(&MeshData);
	RenderingPipeline.BuildMesh(MyMesh, MeshData);

	MyMesh->Init();

	return MyMesh;
}
