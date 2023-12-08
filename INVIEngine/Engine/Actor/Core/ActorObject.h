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
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	virtual void SetPosition(const XMVECTOR& InNewPosition);
	virtual void SetRotation(const fvector_3d& InRotation);
	virtual void SetScale(const fvector_3d& InNewScale);

	virtual void SetForwardVector(const XMFLOAT3& InForward);
	virtual void SetForwardVector(const XMVECTOR& InForward);
	virtual void SetRightVector(const XMFLOAT3& InRight);
	virtual void SetRightVector(const XMVECTOR& InRight);
	virtual void SetUpVector(const XMFLOAT3& InUp);
	virtual void SetUpVector(const XMVECTOR& InUp);

public:
	XMFLOAT3& GetPosition();
	XMFLOAT3& GetRotation();
	XMFLOAT3& GetScale();

	XMFLOAT3& GetForwardVector();
	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();

};

