#pragma once
#include "Mesh/SphereMesh.h"

class GSky : public  GSphereMesh
{
public:
	GSky();

	virtual void Tick(float DeltaTime) override;
};

