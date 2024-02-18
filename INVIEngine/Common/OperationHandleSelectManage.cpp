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

		// ������������Ӷ�λ��ѡ�е�������
		if (GOperationHandle* handle = dynamic_cast<GOperationHandle*>(SelectedObject))
		{
			handle->SetPosition(SelectedActor->GetPosition());
			// handle->SetRotation(SelectedActor->GetRotation());
			// handle->SetScale(SelectedActor->GetScale());
			handle->SetVisible(true);
		}
		// ��¼ѡ�еĲ������
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
			AllOperationHandleHide();
			// ������������Ӷ�λ��ѡ�е�������
			// �ж�SelectedOperationHandle������
			if (GMoveArrow* handle = dynamic_cast<GMoveArrow*>(SelectedOperationHandle))
			{
				MoveArrow->SetPosition(SelectedActor->GetPosition());
				// handle->SetRotation(SelectedActor->GetRotation());
				// handle->SetScale(SelectedActor->GetScale());
				MoveArrow->SetVisible(true);
			}
			else if (GRotateArrow* handle = dynamic_cast<GRotateArrow*>(SelectedOperationHandle))
			{
				RotateArrow->SetPosition(SelectedActor->GetPosition());
				// handle->SetRotation(SelectedActor->GetRotation());
				// handle->SetScale(SelectedActor->GetScale());
				RotateArrow->SetVisible(true);
			}
			else if (GScalingArrow* handle = dynamic_cast<GScalingArrow*>(SelectedOperationHandle))
			{
				ScaleArrow->SetPosition(SelectedActor->GetPosition());
				// handle->SetRotation(SelectedActor->GetRotation());
				// handle->SetScale(SelectedActor->GetScale());
				ScaleArrow->SetVisible(true);
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
