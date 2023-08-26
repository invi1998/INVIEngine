#include "Mesh.h"
#include "EngineMinimal.h"
#include "Component/TransformationComponent.h"
#include "Config/EngineRenderConfig.h"
#include "Material/Core/Material.h"
#include "Platform/Windows/WindowsEngine.h"

/**
 * \brief /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */
GMesh::GMesh() : GActorObject()
{
	Materials.push_back(CreateObject<CMaterial>(new CMaterial()));
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

void GMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
}

UINT GMesh::GetMaterialNumber() const
{
	return Materials.size();
}

