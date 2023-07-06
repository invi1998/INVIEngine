#include "CustomMesh.h"

void CCustomMesh::BeginInit()
{
	CMesh::BeginInit();
}

void CCustomMesh::Tick(float DeltaTime)
{
	CMesh::Tick(DeltaTime);
}

CCustomMesh::~CCustomMesh()
{
}

void CCustomMesh::Init()
{
	CMesh::Init();
}

void CCustomMesh::PreDraw(float DeltaTime)
{
	CMesh::PreDraw(DeltaTime);
}

void CCustomMesh::Draw(float DeltaTime)
{
	CMesh::Draw(DeltaTime);
}

void CCustomMesh::PostDraw(float DeltaTime)
{
	CMesh::PostDraw(DeltaTime);
}

void CCustomMesh::BuildMesh(const FMeshRendingData* InRenderingData)
{
	CMesh::BuildMesh(InRenderingData);
}
