#pragma once

#include "EngineMinimal.h"
#include "Core/CoreObject/CoreMinimalObject.h"


class CTransformationComponent;

// G -> Game
class GActorObject : public CCoreMinimalObject
{
	CVARIABLE()
		CTransformationComponent* TransformationComponent;

public:
	GActorObject();

	FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }

public:
	void SetPosition(const XMFLOAT3& InNewPosition);
	void SetRotation(const fvector_3d& InRotation);
	void SetScale(const fvector_3d& InNewScale);

public:
	XMFLOAT3& GetPosition();
	XMFLOAT3& GetScale();
	XMFLOAT3& GetForwardVector();
	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();

};

