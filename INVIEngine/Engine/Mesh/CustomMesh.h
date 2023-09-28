#pragma once

#include "Core/Mesh.h"


// 自定义网格
class GCustomMesh : public GMesh
{
public:
	void Init() override;
	void Draw(float DeltaTime) override;

	void CreateMesh(const string& InPath);
};

