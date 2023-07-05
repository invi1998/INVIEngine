#pragma once
#include "Core/Mesh.h"

class CSphereMesh : public CMesh
{
public:
	CSphereMesh();
	~CSphereMesh() override;


	void BeginInit() override;
	void Tick(float DeltaTime) override;
	void Init() override;
	void PreDraw(float DeltaTime) override;
	void Draw(float DeltaTime) override;
	void PostDraw(float DeltaTime) override;
	void BuildMesh(const FMeshRendingData* InRenderingData) override;

private:
	float Radius;					// �뾶
	uint32_t AxialSubdivision;			// ����ϸ�֣���ֱ γ�ߣ�
	uint32_t HeightSubdivision;			// �߶�ϸ�֣����� ���ߣ�

	float Theta;
	float Beta;

};

