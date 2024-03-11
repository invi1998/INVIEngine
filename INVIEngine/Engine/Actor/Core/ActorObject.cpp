#include "EngineMinimal.h"

#include "ActorObject.h"

#include "Component/TransformationComponent.h"
#include "Component/Mesh/Core/MeshComponent.h"


GActorObject::GActorObject()
{
	FCreateObjectParams params{};
	params.Owner = this;
	RootComponent = CreateObject<CTransformationComponent>(params, new CTransformationComponent());
}

void GActorObject::GetBoundingBox(BoundingBox& OutBoundingBox) const
{
	if (RootComponent)	// 如果RootComponent不为空
	{
		for (auto& child : RootComponent->GetChildComponents())	// 遍历RootComponent的子组件
		{
			if (CMeshComponent* mesh = dynamic_cast<CMeshComponent*>(child))	// 如果子组件是MeshComponent（因为我们的BoundingBox信息是存储在MeshComponent中的）
			{
				// 获取BoundingBox
				BoundingBox box{};
				mesh->GetBoundingBox(box);
				// 合并BoundingBox，得到最终的BoundingBox（一个ActorObject可能有多个MeshComponent，所以需要合并）
				// 合并BoundingBox的方法是：将两个BoundingBox的最小点和最大点分别取最小值和最大值
				// min = center - extent, max = center + extent
				BoundingBox::CreateMerged(OutBoundingBox, OutBoundingBox, box);
			}
		}
	}
}

BoundingBox GActorObject::GetBoundingBox() const
{
	BoundingBox box{};
	GetBoundingBox(box);
	return box;
}

void GActorObject::SetPosition(const XMFLOAT3& InNewPosition)
{
	RootComponent->SetPosition(InNewPosition);
}

void GActorObject::SetPosition(const XMVECTOR& InNewPosition)
{
	XMFLOAT3 pos{};
	XMStoreFloat3(&pos, InNewPosition);
	SetPosition(pos);
}

void GActorObject::SetRotation(const XMFLOAT3& InRotation)
{
	RootComponent->SetRotation(InRotation);
}

void GActorObject::SetRotation(const frotator& InRotation)
{
	RootComponent->SetRotation(InRotation);
}

void GActorObject::SetScale(const XMFLOAT3& InNewScale)
{
	RootComponent->SetScale(InNewScale);
}

void GActorObject::SetScale(const XMVECTOR& Scale)
{
	XMFLOAT3 scale{};
	XMStoreFloat3(&scale, Scale);
	SetScale(scale);
}

void GActorObject::SetForwardVector(const XMFLOAT3& InForward)
{
	RootComponent->SetForwardVector(InForward);
}

void GActorObject::SetForwardVector(const XMVECTOR& InForward)
{
	XMFLOAT3 fw{};
	XMStoreFloat3(&fw, InForward);
	SetForwardVector(fw);
}

void GActorObject::SetRightVector(const XMVECTOR& InRight)
{
	XMFLOAT3 right{};
	XMStoreFloat3(&right, InRight);
	SetRightVector(right);
}

void GActorObject::SetUpVector(const XMVECTOR& InUp)
{
	XMFLOAT3 up{};
	XMStoreFloat3(&up, InUp);
	SetUpVector(up);
}

void GActorObject::SetRoationQuat(const XMVECTOR& InQuat)
{
	RootComponent->SetRotationQuat(InQuat);
}

void GActorObject::SetRightVector(const XMFLOAT3& InRight)
{
	RootComponent->SetRightVector(InRight);
}

void GActorObject::SetUpVector(const XMFLOAT3& InUp)
{
	RootComponent->SetUpVector(InUp);
}

XMFLOAT3& GActorObject::GetPosition()
{
	return RootComponent->GetPosition();
}

XMFLOAT3& GActorObject::GetRotation()
{
	return RootComponent->GetRotation();
}

XMFLOAT3& GActorObject::GetScale()
{
	return RootComponent->GetScale();
}

XMFLOAT3& GActorObject::GetForwardVector()
{
	return RootComponent->GetForwardVector();
}

XMFLOAT3& GActorObject::GetRightVector()
{
	return RootComponent->GetRightVector();
}

XMFLOAT3& GActorObject::GetUpVector()
{
	return RootComponent->GetUpVector();
}

XMVECTOR GActorObject::GetRotationQuat()
{
	return RootComponent->GetRotationQuat();
}
