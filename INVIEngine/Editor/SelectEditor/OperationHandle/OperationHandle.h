#pragma once
#include "Actor/Core/ActorObject.h"
#include "Component/Mesh/Core/MeshComponentType.h"
#include "Interface/DirectXDeviceInterface.h"

class CInputComponent;
class CCustomMeshComponent;

class GOperationHandle : public GActorObject, public IDirectXDeviceInterface
{
public:
	GOperationHandle();
	virtual ~GOperationHandle() override = default;

	virtual void SetMesh() = 0;

	virtual void SetMeshRenderLayerType(EMeshRenderLayerType mesh_render_layer);

protected:
	CVARIABLE()
	CCustomMeshComponent* XAxisComponent = nullptr;	// X��

	CVARIABLE()
	CCustomMeshComponent* YAxisComponent = nullptr;	// Y��

	CVARIABLE()
	CCustomMeshComponent* ZAxisComponent = nullptr;	// Z��

	CVARIABLE()
	CInputComponent* InputComponent = nullptr;	// �������
};

