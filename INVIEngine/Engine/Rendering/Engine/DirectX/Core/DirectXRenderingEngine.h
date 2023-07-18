#pragma once

#include "EngineMinimal.h"
#include "Mesh/Core/MeshManager.h"
#include "Rendering/Engine/Core/RenderingEngine.h"

class CDirectXRenderingEngine : public CRenderingEngine
{
	friend class IRenderingInterface;

	friend class IDirectXDeviceInterface;

public:
	CDirectXRenderingEngine();
	~CDirectXRenderingEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters) override;
	virtual int Init(FWinMainCommandParameters InParameters) override;
	virtual int PostInit() override;

	virtual void Tick(float DeltaTime) override;

	virtual int PreExit() override;
	virtual int Exit() override;
	virtual int PostExit() override;

public:

	ID3D12Resource* GetCurrentSwapBuffer() const;	// ��ȡ��ǰ������buff������
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;		// ��ȡ��ǰ������buffer���ӿھ��
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;		// ��ȡ��ǰ���ģ��View

	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }			// ��ȡ��̨��������ʽ
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; }		// ��ȡ���ģ�建������ʽ
	UINT GetDXGISampleCount() const;												// ��ȡ��������
	UINT GetDXGISampleQuality() const;												// ��ȡ��������

protected:
	void WaitGPUCommandQueueComplete();		// �ȴ�GPUִ��������� ͬ��GPU��CPU
	// ��ʼ��Direct3D
	bool InitDirect3D();
	void PostInitDirect3D();

protected:
	UINT64 CurrentFenceIndex;		// ��ǰΧ������
	int CurrentSwapBufferIndex;		// ��ǰ������buffer����������������������������������ʱ�򣬾ͻ�ȥ�޸����ֵ��

	// ��������
	ComPtr<IDXGIFactory4> DXGiFactory;	// ����DirectXͼ�λ����ṹ��DXGi������
	ComPtr<ID3D12Device> D3dDevice;		// ��������������������б���������У�Fence����Դ���ܵ�״̬���󣬶ѣ���ǩ������������������Դ��ͼ
	ComPtr<ID3D12Fence> Fence;			// һ������ͬ��CPU��һ�����߶��GPU�Ķ���

	// �������
	ComPtr<ID3D12CommandQueue> CommandQueue;				// �������
	ComPtr<ID3D12CommandAllocator> CommandAllocator;		// ��������������洢����ͷ����ڴ棩
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;	// ͼ�������б�

	// ����������
	ComPtr<IDXGISwapChain> SwapChain;		// ������

	// ����������Ͷ�
	ComPtr<ID3D12DescriptorHeap> RTVHeap;		// ��ȾĿ����ͼ
	ComPtr<ID3D12DescriptorHeap> DSVHeap;		// ���ģ����ͼ

	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;		// ������Buffer������������������һ��ǰ̨��������һ����̨��������
	ComPtr<ID3D12Resource> DepthStencilBuffer;					// ���ģ�建����

	// ����Ļ�ӿ����
	D3D12_VIEWPORT ViewPortInfo;		// �ӿ���Ϣ
	D3D12_RECT ViewPortRect;			// �ӿڲü�����

protected:

	// ��ǰ��������������(4����������������
	UINT M4XNumQualityLevels;

	// 4�ز����Ƿ���
	bool bMSAA4XEnabled;

	// ����buffer��ʽ
	DXGI_FORMAT BackBufferFormat;	// ��̨��������ʽ
	DXGI_FORMAT DepthStencilFormat; // ���ģ�建������ʽ

	UINT RTVDescriptorSize;			// RTV��������С

	CMeshManager* MeshManage;

};
