#include "Mesh.h"

#include "Component/Mesh/ShellMeshComponent.h"
#include "Material/Core/Material.h"

/**
 * \brief /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
GMesh::GMesh() : GActorObject()
{
	MeshComponent = CreateObject<CMeshComponent>(new CMeshComponent());
}

GMesh::~GMesh()
{
}

void GMesh::Init()
{
	if (MeshComponent)
	{
		MeshComponent->Init();
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
	if (MeshComponent)
	{
		MeshComponent->BuildMesh(InRenderingData);
	}
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

	//��Ϊ��δ�����������ʱ��ôд
	GetMeshComponent()->SetPosition(InNewPosition);
}

void GMesh::SetRotation(const fvector_3d& InRotation)
{
	GActorObject::SetRotation(InRotation);

	//��Ϊ��δ�����������ʱ��ôд
	GetMeshComponent()->SetRotation(InRotation);
}

void GMesh::SetScale(const fvector_3d& InNewScale)
{
	GActorObject::SetScale(InNewScale);

	//��Ϊ��δ�����������ʱ��ôд
	GetMeshComponent()->SetScale(InNewScale);
}

void GMesh::SetMeshComponent(CMeshComponent* InMeshComponent)
{
	MeshComponent = InMeshComponent;
}


