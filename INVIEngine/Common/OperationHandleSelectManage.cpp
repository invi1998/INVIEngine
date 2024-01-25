#include "EngineMinimal.h"
#include "OperationHandleSelectManage.h"

#include "Actor/Core/ActorObject.h"
#include "SelectEditor/OperationHandle/MoveArrow.h"

extern class GMoveArrow* MoveArrow;
//extern class GRotateArrow* RotateArrow;
//extern class GScaleArrow* ScaleArrow;
extern class GActorObject* SelectedActor;

FOperationHandleSelectManage::FOperationHandleSelectManage()
{
}

FOperationHandleSelectManage::~FOperationHandleSelectManage()
{
}

FOperationHandleSelectManage* FOperationHandleSelectManage::GetInstance()
{
}

void FOperationHandleSelectManage::Destroy()
{
}

void FOperationHandleSelectManage::AllOperationHandleHide()
{
}

void FOperationHandleSelectManage::DisplaySelectedHandle(GActorObject* SelectedObject)
{
}

void FOperationHandleSelectManage::DisplaySelectedHandle()
{
}

void FOperationHandleSelectManage::HideSelectedHandle()
{
}

void FOperationHandleSelectManage::SetNewSelectedOperationHandle(GActorObject* SelectedObject)
{
}

void FOperationHandleSelectManage::SetNewSelectedSelectedObject()
{
}

GActorObject* FOperationHandleSelectManage::GetSelectedOperationHandle()
{
}
