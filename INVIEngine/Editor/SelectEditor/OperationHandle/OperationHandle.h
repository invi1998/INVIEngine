#pragma once
#include "OperationHandleSelectManage.h"
#include "Actor/Core/ActorObject.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Interface/DirectXDeviceInterface.h"

class CInputComponent;
class CCustomMeshComponent;

// ѡ�е�����
enum ESelectedAxis
{
	AXIS_NONE = 0,
	AXIS_X = 1,		// ��Ӧ�����Right����
	AXIS_Y = 2,		// ��Ӧ�����Up����
	AXIS_Z = 3,		// ��Ӧ�����Forward����
	AXIS_ANY = 4,	// ���ⷽ��
};

class GOperationHandle : public GActorObject, public IDirectXDeviceInterface
{
public:
	GOperationHandle();
	virtual ~GOperationHandle() override = default;

	virtual void SetMesh() = 0;

	virtual void SetMeshRenderLayerType(EMeshRenderLayerType mesh_render_layer);

	void ResetColor();

	void ResetColor(CCustomMeshComponent* axis_component, const XMFLOAT4& color);

	void BeginInit() override;

	virtual void ExecuteInput() = 0;		// �ýӿ���Ҫ���������������¼���Ҫ������������ʵ��

	void Tick(float DeltaTime) override;

	void SetPosition(const XMVECTOR& InNewPosition) override;

	void SetPosition(const XMFLOAT3& InNewPosition) override;

	void SetVisible(bool visible);

	void SetVisible(bool visible, CCustomMeshComponent* axis_component);

	void SetScale(const XMFLOAT3& InNewScale) override;

	bool IsVisible();

	ESelectedAxis GetSelectedAxis() const;

	void SetWorldOperate(bool bWorld) { bWorldOperate = bWorld; }

	bool IsWorldOperate() const { return bWorldOperate; }

protected:
	CVARIABLE()
	CCustomMeshComponent* XAxisComponent = nullptr;	// X��

	CVARIABLE()
	CCustomMeshComponent* YAxisComponent = nullptr;	// Y��

	CVARIABLE()
	CCustomMeshComponent* ZAxisComponent = nullptr;	// Z��

	CVARIABLE()
	CCustomMeshComponent* AnyAxisComponent = nullptr;	// ������

	CVARIABLE()
	CInputComponent* InputComponent = nullptr;	// �������

	virtual void OnMouseMoved(int x, int y);

	virtual void OnMouseLeftDown(int x, int y);

	virtual void OnMouseLeftUp(int x, int y);

	// ��Ҫ����ʵ�ֵĻ�ȡ�����ᷴ����麯����������������ϵ��ԭ��ͷ����ʵ��λ�ã�����������ķ���
	virtual void GetAnyAxisDirection(XMVECTOR& WorldOriginPoint, XMVECTOR& WorldDirection, XMVECTOR& ActorLocation, XMVECTOR& OutDirection) = 0;

	float GetMouseMoveDistance(int x, int y, XMVECTOR& ActorLocation, XMVECTOR& DragDirection);

	bool IsCurrentSelectedHandle();

	// �ж�ѡ��Ĳ�������Ƿ�ǰ�ض�������
	template<typename T>
	bool IsCurrentSelectedHandleType()
	{
		return dynamic_cast<T*>(this) != FOperationHandleSelectManage::Get()->GetSelectedOperationHandle();
	}

private:
	ESelectedAxis SelectedAxis = AXIS_NONE;	// ѡ�е�����

	float FixedZoom = 30.f;	// �̶������ű���

	// �Ƿ���������ϵ����
	bool bWorldOperate = true;
};

