#include "Material.h"

CMaterial::CMaterial()
	: BaseColor(0.18f, 0.15f, 0.13f, 1.f)
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
