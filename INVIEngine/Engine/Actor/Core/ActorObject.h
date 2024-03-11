#pragma once

#include "EngineMinimal.h"
#include "Core/CoreObject/CoreMinimalObject.h"


class CTransformationComponent;

// G -> Game
class GActorObject : public CCoreMinimalObject
{
	CVARIABLE()
		CTransformationComponent* RootComponent = nullptr;	// 空指针异常会导致程序崩溃

public:
	GActorObject();

	FORCEINLINE CTransformationComponent* GetRootComponent() const { return RootComponent; }

	// 获取BoundingBox
	void GetBoundingBox(BoundingBox& OutBoundingBox) const;
	BoundingBox GetBoundingBox() const;

public:
	virtual void SetPosition(const XMFLOAT3& InNewPosition);
	virtual void SetPosition(const XMVECTOR& InNewPosition);
	virtual void SetRotation(const XMFLOAT3& InRotation);
	virtual void SetRotation(const frotator& InRotation);
	virtual void SetScale(const XMFLOAT3& InNewScale);
	virtual void SetScale(const XMVECTOR& Scale);

	virtual void SetForwardVector(const XMFLOAT3& InForward);
	virtual void SetForwardVector(const XMVECTOR& InForward);
	virtual void SetRightVector(const XMFLOAT3& InRight);
	virtual void SetRightVector(const XMVECTOR& InRight);
	virtual void SetUpVector(const XMFLOAT3& InUp);
	virtual void SetUpVector(const XMVECTOR& InUp);
	virtual void SetRoationQuat(const XMVECTOR& InQuat);
	virtual void SetRoationFQuat(const fquat& InQuat);

public:
	XMFLOAT3& GetPosition();
	XMFLOAT3& GetRotation();
	XMFLOAT3& GetScale();

	XMFLOAT3& GetForwardVector();
	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();

	XMVECTOR GetRotationQuat();
	frotator GetRotationFrotator();
	fquat GetRotationFQuat();
	
};

