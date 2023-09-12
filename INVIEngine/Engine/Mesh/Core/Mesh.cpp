#include "Mesh.h"

#include "Component/Mesh/ShellMeshComponent.h"
#include "Material/Core/Material.h"

/**
 * \brief /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
GMesh::GMesh() : GActorObject()
{
	ShellMeshComponent = CreateObject<CShellMeshComponent>(new CShellMeshComponent());
}

GMesh::~GMesh()
{
}

void GMesh::Init()
{
	if (ShellMeshComponent)
	{
		ShellMeshComponent->Init();
	}
}

void GMesh::PreDraw(float DeltaTime)
{
	
}

void GMesh::Draw(float DeltaTime)
{

	
}

void GMesh::PostDraw(float DeltaTime)
{
}

void GMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
	if (ShellMeshComponent)
	{
		ShellMeshComponent->BuildMesh(InRenderingData);
	}
}

UINT GMesh::GetMaterialNum() const
{
	return ShellMeshComponent->GetMaterialNum();
}

std::vector<CMaterial*>* GMesh::GetMaterial()
{
	return ShellMeshComponent->GetMaterial();
}


