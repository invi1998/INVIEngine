#pragma once

class GActorObject;

class FOperationHandleSelectManage
{
public:
	FOperationHandleSelectManage(void);
	~FOperationHandleSelectManage(void);

	static FOperationHandleSelectManage* Get();

	static void Destroy();

	void AllOperationHandleHide();	//隐藏所有操作句柄

	void DisplaySelectedHandle(GActorObject* SelectedObject);		//显示选择句柄
	void DisplaySelectedHandle();		//显示选择句柄
	void HideSelectedHandle();		//隐藏选择句柄

	void SetNewSelectedOperationHandle(GActorObject* actor);		//显示新的操作句柄
	void SetNewSelectedObject(GActorObject* actor);		//设置新的选择对象

	GActorObject* GetSelectedOperationHandle();		//获取选中的操作句柄

private:
	static FOperationHandleSelectManage* Instance;

	GActorObject* SelectedOperationHandle = nullptr;		//选中的操作句柄
	GActorObject* SelectedObject = nullptr;		//选中的对象

};

