#pragma once
#include "OperationHandle.h"

class CMeshComponent;
class CPlaneMeshComponent;

class GRotateArrow : public GOperationHandle
{
	CVARIABLE()
	CPlaneMeshComponent* XPlaneComponent = nullptr;

	CVARIABLE()
	CPlaneMeshComponent* YPlaneComponent = nullptr;

	CVARIABLE()
	CPlaneMeshComponent* ZPlaneComponent = nullptr;


public:
	GRotateArrow();

	void SetMesh() override;

	void OnMouseMoved(int x, int y) override;

	void OnMouseLeftDown(int x, int y) override;

	void OnMouseLeftUp(int x, int y) override;

	void ExecuteInput() override;

	void OnMousePressed(int x, int y);

	void SetScale(const XMFLOAT3& InNewScale) override;

	void SetPosition(const XMFLOAT3& InNewPosition) override;

	void SetVisible() override;

	void SetVisible(bool visible) override;

	void SetVisible(bool visible, CCustomMeshComponent* axis_component) override;

	void Tick(float DeltaTime) override;

	void SetCDValue(float InCDValue);

	void SetCDValue(float InCDValue, CMeshComponent* axis_component);

	void ResetCDValue();

protected:
	XMVECTOR GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation) override;
	void SetMaterial(CMeshComponent* axis_component, const std::string& path);

	void GetSelectedObjectDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation, XMVECTOR& DragDirection) override;

protected:
	float LastT2Value = 0.0f;	// ��һ�ε�T2ֵ
	float RotateRadio = 0.f;	// ��ת��ֵ
	int Sample8CubeIndex = 0;	// ����8�������������
};


// ��Ԫ����ת��������
// ʵ��˼·��
// 1: ��ȡ�������ת
// 2: ���������תת��Ϊ��Ԫ��
// 3�����������ק����ķ����������������ת����Ԫ������Ԫ������ת�����ת�Ƕȣ�
// 4�����������ת��Ԫ���ͼ��������ת��Ԫ����ˣ��õ��µ���ת��Ԫ��
// 5�����µ���ת��Ԫ��ת��Ϊŷ���ǣ������������ת
