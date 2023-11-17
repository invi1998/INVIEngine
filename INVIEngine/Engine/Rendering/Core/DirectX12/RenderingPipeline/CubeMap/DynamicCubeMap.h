#pragma once
#include "Interface/DirectXDeviceInterface.h"

struct FDirectXPipelineState;
struct FGeometryMap;
class GClientViewPort;
class FCubeMapRenderTarget;

// ��̬CubeMap
class FDynamicCubeMap : public IDirectXDeviceInterface
{
public:
	FDynamicCubeMap();
	virtual void Init(FGeometryMap* inGeometryMap, FDirectXPipelineState* inDirectXPipelineState);

protected:
	// �����ӿ�
	virtual void BuildViewPort(const XMFLOAT3& CenterPoint);

	// �������ģ��
	virtual void BuildDepthStencil();

protected:
	std::unique_ptr<FCubeMapRenderTarget> CubeMapRenderTarget{};	// renderTarget ��ȾĿ��
	CD3DX12_CPU_DESCRIPTOR_HANDLE DSVCubeMapCPUDesc;			// BSV���� ���ģ������

	std::vector<GClientViewPort*> CubeMapViewPorts{};		// CubeMap�ӿ� 6��

	FGeometryMap* GeometryMap = nullptr;	// ����
	FDirectXPipelineState* DirectXPipelineState = nullptr;	// ��Ⱦ����״̬����
};

