#include "Mesh.h"
#include "EngineMinimal.h"
#include "Component/TransformationComponent.h"
#include "Config/EngineRenderConfig.h"
#include "Platform/Windows/WindowsEngine.h"

/**
 * \brief /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
CMesh::CMesh()
{
	TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

CMesh::~CMesh()
{
}

void CMesh::Init()
{
	

}

void CMesh::PreDraw(float DeltaTime)
{
	
}

void CMesh::Draw(float DeltaTime)
{

	
}

void CMesh::PostDraw(float DeltaTime)
{
}

void CMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

void CMesh::SetPosition(const XMFLOAT3& InNewPosition)
{
	TransformationComponent->SetPosition(InNewPosition);
}

void CMesh::SetRotation(const fvector_3d& InRotation)
{
}

void CMesh::SetScale(const fvector_3d& InNewScale)
{
	TransformationComponent->SetScale(InNewScale);
}

XMFLOAT3& CMesh::GetPosition()
{
	return TransformationComponent->GetPosition();
}

XMFLOAT3 CMesh::GetForwardVector()
{
	return TransformationComponent->GetForwardVector();
}

XMFLOAT3 CMesh::GetRightVector()
{
	return TransformationComponent->GetRightVector();
}

XMFLOAT3 CMesh::GetUpVector()
{
	return TransformationComponent->GetUpVector();
}

