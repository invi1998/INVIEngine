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

	void SetNewSelectedOperationHandle(GActorObject* SelectedObject);		//��ʾ�µĲ������
	void SetNewSelectedSelectedObject();		//�����µ�ѡ�����

	GActorObject* GetSelectedOperationHandle();		//��ȡѡ�еĲ������

private:
	static FOperationHandleSelectManage* Instance;

	GActorObject* SelectedOperationHandle;		//ѡ�еĲ������
	GActorObject* SelectedObject;		//ѡ�еĶ���

};

