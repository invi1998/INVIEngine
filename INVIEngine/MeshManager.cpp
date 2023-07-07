#include "MeshManager.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

void CMeshManager::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void CMeshManager::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);
}

void CMeshManager::Init()
{
	IRenderingInterface::Init();
}

void CMeshManager::PreDraw(float DeltaTime)
{
	IRenderingInterface::PreDraw(DeltaTime);
}

void CMeshManager::Draw(float DeltaTime)
{
	IRenderingInterface::Draw(DeltaTime);
}

void CMeshManager::PostDraw(float DeltaTime)
{
	IRenderingInterface::PostDraw(DeltaTime);
}
