#include "MeshComponent.h"

#include "Material/Core/Material.h"

CMeshComponent::CMeshComponent()
{
	Materials.push_back(CreateObject<CMaterial>(new CMaterial()));
}

UINT CMeshComponent::GetMaterialNum() const
{
	return Materials.size();
}
