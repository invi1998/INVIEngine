#pragma once

class GActorObject;

class FOperationHandleSelectManage
{
public:
	FOperationHandleSelectManage(void);
	~FOperationHandleSelectManage(void);

	static FOperationHandleSelectManage* Get();

	static void Destroy();

	void AllOperationHandleHide();	//�������в������

	void DisplaySelectedHandle(GActorObject* SelectedObject);		//��ʾѡ����
	void DisplaySelectedHandle();		//��ʾѡ����
	void HideSelectedHandle();		//����ѡ����

	void SetNewSelectedOperationHandle(GActorObject* actor);		//��ʾ�µĲ������
	void SetNewSelectedObject(GActorObject* actor);		//�����µ�ѡ�����

	GActorObject* GetSelectedOperationHandle();		//��ȡѡ�еĲ������

private:
	static FOperationHandleSelectManage* Instance;

	GActorObject* SelectedOperationHandle = nullptr;		//ѡ�еĲ������
	GActorObject* SelectedObject = nullptr;		//ѡ�еĶ���

};

