#pragma once

#include "Core/Mesh.h"


// �Զ�������
class GCustomMesh : public GMesh
{
public:
	void Init() override;
	void Draw(float DeltaTime) override;

	void CreateMesh(const string& InPath);
};

