#include "EngineMinimal.h"
#include "Sky.h"

#include "Material/Core/Material.h"
#include "Material/Core/MaterialType.h"

GSky::GSky():GSphereMesh()
{
	SetMeshRenderLayerType(EMeshRenderLayerType::RENDER_LAYER_BACKGROUND);

	
	CreateMesh(2.f, 100, 100, true);
	GMesh::SetScale(fvector_3d(4000.f));
	if (CMaterial* InMaterial = (*GetMaterial())[0])
	{
		InMaterial->SetBaseColor(XMFLOAT4(0.f, 0.f, 0.f, 1.f));
		InMaterial->SetBaseColorIndexKey("x1_CubeMap");
		InMaterial->SetSpecular(XMFLOAT3(1.f, 1.f, 1.f));
		InMaterial->SetMaterialType(EMaterialType::BaseColor);
	}

}

void GSky::Tick(float DeltaTime)
{
	GSphereMesh::Tick(DeltaTime);
}
