#pragma once

#include "EngineMinimal.h"

class RowCamera
{
public:
	RowCamera() = default;
	RowCamera(const XMFLOAT4X4& projection) : m_Projection(projection) {}
	virtual ~RowCamera() = default;

	const XMFLOAT4X4& GetProjection() const { return m_Projection; }

protected:
	XMFLOAT4X4 m_Projection = EngineMath::IdentityMatrix4x4();
};
