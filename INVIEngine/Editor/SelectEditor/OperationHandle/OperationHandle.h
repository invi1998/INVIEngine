#pragma once
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

	void ExecuteInput();

	void Tick(float DeltaTime) override;

	void SetPosition(const XMVECTOR& InNewPosition) override;

	void SetPosition(const XMFLOAT3& InNewPosition) override;

	ESelectedAxis GetSelectedAxis() const;

protected:
	CVARIABLE()
	CCustomMeshComponent* XAxisComponent = nullptr;	// X��

	CVARIABLE()
	CCustomMeshComponent* YAxisComponent = nullptr;	// Y��

	CVARIABLE()
	CCustomMeshComponent* ZAxisComponent = nullptr;	// Z��

	CVARIABLE()
	CInputComponent* InputComponent = nullptr;	// �������

	virtual void OnMouseMoved(int x, int y);

	virtual void OnMouseLeftDown(int x, int y);

	virtual void OnMouseLeftUp(int x, int y);

private:
	ESelectedAxis SelectedAxis = AXIS_NONE;	// ѡ�е�����
};

