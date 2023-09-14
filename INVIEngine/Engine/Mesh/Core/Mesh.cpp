#include "Mesh.h"

#include "Component/Mesh/ShellMeshComponent.h"
#include "Material/Core/Material.h"

/**
 * \brief /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
GMesh::GMesh() : GActorObject()
{
}

GMesh::~GMesh()
{
}

void GMesh::Init()
{
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

UINT GMesh::GetMaterialNum() const
{
	return MeshComponent->GetMaterialNum();
}

std::vector<CMaterial*>* GMesh::GetMaterial()
{
	return MeshComponent->GetMaterial();
}

void GMesh::SetPosition(const XMFLOAT3& InNewPosition)
{
	GActorObject::SetPosition(InNewPosition);

	//因为还未做根组件，暂时这么写
	GetMeshComponent()->SetPosition(InNewPosition);
}

void GMesh::SetRotation(const fvector_3d& InRotation)
{
	GActorObject::SetRotation(InRotation);

	//因为还未做根组件，暂时这么写
	GetMeshComponent()->SetRotation(InRotation);
}

void GMesh::SetScale(const fvector_3d& InNewScale)
{
	GActorObject::SetScale(InNewScale);

	//因为还未做根组件，暂时这么写
	GetMeshComponent()->SetScale(InNewScale);
}

void GMesh::SetMeshComponent(CMeshComponent* InMeshComponent)
{
	MeshComponent = InMeshComponent;
}


