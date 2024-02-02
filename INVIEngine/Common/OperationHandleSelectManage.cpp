#include "EngineMinimal.h"
#include "OperationHandleSelectManage.h"

#include "Actor/Core/ActorObject.h"
#include "SelectEditor/OperationHandle/MoveArrow.h"
#include "SelectEditor/OperationHandle/RotateArrow.h"
#include "SelectEditor/OperationHandle/ScalingArrow.h"

extern class GMoveArrow* MoveArrow;
extern class GRotateArrow* RotateArrow;
extern class GScalingArrow* ScaleArrow;
extern class GActorObject* SelectedActor;

FOperationHandleSelectManage* FOperationHandleSelectManage::Instance = nullptr;

FOperationHandleSelectManage::FOperationHandleSelectManage()
{
}

FOperationHandleSelectManage::~FOperationHandleSelectManage()
{
}

FOperationHandleSelectManage* FOperationHandleSelectManage::Get()
{
	if (Instance == nullptr)
	{
		Instance = new FOperationHandleSelectManage();
	}
	return Instance;
}

void FOperationHandleSelectManage::Destroy()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

void FOperationHandleSelectManage::AllOperationHandleHide()
{
	MoveArrow->SetVisible(false);
	ScaleArrow->SetVisible(false);
	RotateArrow->SetVisible(false);
}

void FOperationHandleSelectManage::DisplaySelectedHandle(GActorObject* SelectedObject)
{
	if (SelectedActor)
	{
		AllOperationHandleHide();

		// 将操作句柄附加定位到选中的物体上
		if (GOperationHandle* handle = dynamic_cast<GOperationHandle*>(SelectedObject))
		{
			handle->SetPosition(SelectedActor->GetPosition());
			// handle->SetRotation(SelectedActor->GetRotation());
			// handle->SetScale(SelectedActor->GetScale());
			handle->SetVisible(true);
		}
		// 记录选中的操作句柄
		SetNewSelectedOperationHandle(SelectedObject);
	}
}

void FOperationHandleSelectManage::DisplaySelectedHandle()
{
	if (SelectedActor)
	{
		if (!SelectedOperationHandle)
		{
			if (MoveArrow)
			{
				MoveArrow->SetPosition(SelectedActor->GetPosition());
				MoveArrow->SetVisible(true);

				SetNewSelectedOperationHandle(MoveArrow);
			}
		}
		else
		{
			if (GOperationHandle* handle = dynamic_cast<GOperationHandle*>(SelectedOperationHandle))
			{
				handle->SetPosition(SelectedActor->GetPosition());
				handle->SetVisible(true);
			}
		}
	}
}

void FOperationHandleSelectManage::HideSelectedHandle()
{
	if (GOperationHandle* handle = dynamic_cast<GOperationHandle*>(SelectedOperationHandle))
	{
		handle->SetVisible(false);
	}
}

void FOperationHandleSelectManage::SetNewSelectedOperationHandle(GActorObject* actor)
{
	SelectedOperationHandle = actor;
}

void FOperationHandleSelectManage::SetNewSelectedObject(GActorObject* actor)
{
	SelectedObject = actor;
}

GActorObject* FOperationHandleSelectManage::GetSelectedOperationHandle()
{
	return SelectedOperationHandle;
}
