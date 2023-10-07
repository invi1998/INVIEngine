#include "EngineMinimal.h"

#include "Material.h"

CMaterial::CMaterial()
	: bDirty(true), MaterialID(-1), BaseColor(0.45f, 0.45f, 0.45f, 1.f), Roughness(0.2f), MaterialTransformation(EngineMath::IdentityMatrix4x4())
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void CMaterial::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);
}

void CMaterial::SetBaseColor(const XMFLOAT4& color)
{
	BaseColor = color;
	SetDirty(true);
}

void CMaterial::SetMaterialType(const EMaterialType type)
{
	MaterialType = type;
	SetDirty(true);
}

void CMaterial::SetRoughness(const float roughness)
{
	Roughness = roughness;
	SetDirty(true);
}

void CMaterial::SetMaterialDisplayStatus(EMaterialDisplayStatusType type)
{
	MaterialDisplayType = type;
	SetDirty(true);
}

void CMaterial::SetTransformation(const XMFLOAT4X4& transform)
{
	MaterialTransformation = transform;
	SetDirty(true);
}

void CMaterial::SetBaseColorIndexKey(const std::string& key)
{
	BaseColorIndexKey = key;
	SetDirty(true);
}

void CMaterial::SetDirty(bool dirty)
{
	bDirty = dirty;
}

void CMaterial::SetMaterialID(const int id)
{
	MaterialID = id;
	SetDirty(true);
}
