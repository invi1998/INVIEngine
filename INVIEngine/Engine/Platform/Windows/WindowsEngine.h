#pragma once

#include "Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
	virtual int PreInit(FWinMainCommandParameters InParameters) override;
	virtual int Init() override;
	virtual int PostInit() override;

	virtual void Tick() override;

	virtual int PreExit() override;
	virtual int Exit() override;
	virtual int PostExit() override;

protected:
	ComPtr<IDXGIFactory4> DXGiFactory;	// ����DirectXͼ�λ����ṹ��DXGi������
	ComPtr<ID3D12Device> D3dDevice;		// ��������������������б�������У�Fence����Դ���ܵ�״̬���󣬶ѣ���ǩ�����������������Դ��ͼ
	ComPtr<ID3D12Fence> Fence;			// һ������ͬ��CPU��һ�����߶��GPU�Ķ���

private:
	// ��ʼ������
	bool InitWindows(FWinMainCommandParameters InParameters);

	// ��ʼ��Direct3D
	bool InitDirect3D();

};
