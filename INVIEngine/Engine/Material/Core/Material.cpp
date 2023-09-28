#include "EngineMinimal.h"

#include "Material.h"

CMaterial::CMaterial()
	: BaseColor(0.45f, 0.45f, 0.45f, 1.f), Roughness(0.2f), MaterialTransformation(EngineMath::IdentityMatrix4x4())
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
