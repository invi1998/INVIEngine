#include "WindowsEngine.h"

#include "WindowsMessageProcessing.h"
#include "Debug/EngineDebug.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// ��������


	// �����ӿ�
	if (InitWindows(InParameters))
	{
		
	}


	if (InitDirect3D())
	{
		
	}



	return 0;
}

int FWindowsEngine::Init()
{
	return 0;
}

int FWindowsEngine::PostInit()
{
	return 0;
}

void FWindowsEngine::Tick()
{
	
}

int FWindowsEngine::PreExit()
{
	return 0;
}

int FWindowsEngine::Exit()
{
	return 0;
}

int FWindowsEngine::PostExit()
{
	return 0;
}

bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	// �󶨴��ڲ���
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);	// �ö���ʵ��ռ�õ��ڴ�
	WindowsClass.cbClsExtra = 0;				// �Ƿ���Ҫ����ռ� ��ʾ����������ڴ��С����ÿ�����ڴ���ʱ��������ô�����Ķ����ڴ��С
	WindowsClass.cbWndExtra = 0;				// �Ƿ���Ҫ�����ڴ� ��ʾ����ʵ�������ڴ��С����Ϊÿ������ʵ������Ķ����ڴ��С
	// cbClsExtra�����ڶ���һ��������Ķ������ݵĴ�С����cbWndExtra�����ڶ���һ������ʵ���Ķ������ݵĴ�С��ͨ���������߿���ʹ����Щ������ڴ����洢һЩ�Զ������ݻ�ָ�룬�Ա��ڴ�����Ϣ����������ʱ���з��ʡ�
	WindowsClass.hbrBackground = nullptr;		// ����ָ�����ڱ�����ʹ�õĻ�ˢ��������û�о�������GDI����
	WindowsClass.hCursor = LoadCursor(nullptr, IDC_ARROW);	// ���ü�ͷ���
	WindowsClass.hIcon = nullptr;				// (����Ӧ�ó�����ڴ��̵�һ��ͼ�꣩
	WindowsClass.hIconSm = nullptr;				// Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance;	// ����ʵ��
	WindowsClass.lpszClassName = L"INVIEngine";	// ��������
	WindowsClass.lpszMenuName = nullptr;		// �˵�����
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;	// ��λ��Ƶ�ǰ���ڣ���ֱ��ˮƽ���ƣ�
	WindowsClass.lpfnWndProc = EngineWidowProc;		// ��Ϣ������

	// ע�ᴰ��
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);

	if (!RegisterAtom)
	{
		MessageBox(NULL, L"Register fail,", L"Error", MB_OK);
	}

	// �ӿڣ��ӿڷ��û�в˵�
	RECT Rect = { 0, 0, 1280, 720 };
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int windowWidth = Rect.right - Rect.left;
	int windowHeight = Rect.bottom - Rect.top;

	HWND hwnd = CreateWindowEx(
		NULL,						// ���ڶ�����
		L"INVIEngine",				// ��������
		L"INVI Engine",				// ��ʾ���������ϵĴ�������
		WS_OVERLAPPEDWINDOW,		// �ӿڷ��
		CW_USEDEFAULT, CW_USEDEFAULT,		// �������꣬����ΪCW_USEDEFAULT�ô�������Ӧ��С������ʾ����Ļ����
		windowWidth,				// ���ڿ��
		windowHeight,				// ���ڸ߶�
		nullptr,					// �����ھ��
		nullptr,					// �˵����
		InParameters.HInstance,		// ����ʵ��
		NULL						// ���ݽ����ڵĶ������
	);

	if (!hwnd)
	{
		// ���ڴ���ʧ��
		MessageBox(0, L"���ڴ���ʧ��", 0, 0);
		return false;
	}

	// ��ʾ����
	ShowWindow(hwnd, SW_SHOW);

	// ˢ�´���
	UpdateWindow(hwnd);
}

bool FWindowsEngine::InitDirect3D()
{
	// HRESULT
	// S_OK					0x00000000	ִ�гɹ�
	// E_UNEXPECTED			0x8000FFFF	�����ʧ��
	// E_NOTIMPL			0x80004001	δʵ��
	// E_OUTOFMEMORY		0x8007000E	δ�ܷ��������ڴ�
	// E_INVALIDARG			0x80070057	һ�����߶��������Ч
	// E_NOINTERFACE		0x80004002	��֧�ִ˽ӿ�
	// E_POINTER			0x80004003	��Чָ��
	// E_HANDLE				0x80070006	��Ч���
	// E_ABORT				0x80004004	������ֹ
	// E_ACCESSDENIED		0x80070005	һ��ķ��ʱ��ܾ�����

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����DGI�豸����

	// ����DGIʵ��
	ANALYSIS_RESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGiFactory)));

	// ��������
	//	IUnknown * pAdapter,          // ����Ҫʹ�õ�������(�豸)ʵ��ָ�룬���Ϊnullptr��ʹ��Ĭ��������
	//	D3D_FEATURE_LEVEL          MinimumFeatureLevel,// ���֧�ֵ����Լ���
	//	REFIID                     riid,               // ϣ��ͨ���˺����������豸�ӿ����� ID
		// D3D_FEATUREI_LEVEL_9_ 1 Ŀ�깦�ܼ���֧��Direct3D 9.1  ����shader model 2.
		// D3D_FEATURE_LEVEL_9_2   Ŀ�깦�ܼ���֧��Direct3D 9.2  ����shader model 2.
		// D3D_FEATURE_LEVEL_9_3   Ŀ�깦�ܼ���֧��Direct3D 9.3  ����shader model 3.
		// D3D_FEATURE_LEVEL_10_0  Ŀ�깦�ܼ���֧��Direct3D 10.0 ����shader model 4.
		// D3D_FEATURE_LEVEL_10_1  Ŀ�깦�ܼ���֧��Direct3D 10.1 ����shader model 4.
		// D3D_FEATURE_LEVEL_11_0  Ŀ�깦�ܼ���֧��Direct3D 11.0 ����shader model 5 .

	//	void** ppvDevice        // ���ص�ָ���豸�ӿڵ�ָ���ָ��
	/*pAdapter��һ��ָ��IDXGIAdapter1����IDXGIAdapter4��ָ�룬��ʾ����ϣ��ʹ�õ����������豸��ʵ��������ò���Ϊnullptr����Direct3D 12�᳢��ʹ��Ĭ�ϵ���������

		MinimumFeatureLevel�����֧�ֵ����Լ��𣬱�ʾӦ�ó�����Ҫʹ�õ�Ӳ������ˮƽ���ò�����ֵӦ����ö������D3D_FEATURE_LEVEL�е�һ����Ա������D3D_FEATURE_LEVEL_11_0��

		riid��һ��Ψһ��ʶ�豸�ӿ����͵�GUID���ò���ͨ����������ΪIID_ID3D12Device����ʾ������Ҫ����һ��ID3D12Device����

		ppvDevice�����ص�ָ���豸�ӿڵ�ָ���ָ�롣�ں����ɹ�ִ�к󣬸�ָ�뽫ָ��һ��ID3D12Device�ӿڵ�ʵ��*/
	HRESULT D3dDeviceResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// ʧ�ܣ�Ӳ������������ʧ�ܣ�
		// Ӳ������������ʧ�ܣ���ô����Ͳ������������
		// warp �߼���դ��ƽ̨

		// ����������
		ComPtr<IDXGIAdapter> WARPAdapter;
		// ö��������
		ANALYSIS_RESULT(DXGiFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		// ����������ʹ�����ģ���������������������
		ANALYSIS_RESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	// ����fence(����Χ����ΪCPU��GPUͬ����׼��
	// ���������������£�
	// InitialValue��Χ����ʼֵ����ʾΧ��״̬����GPUִ�������б�ʱ����д��һ���ź�ֵ��Χ���У�CPU����ͨ������ֵ��ȷ��GPU�Ƿ��Ѿ��������Ⱦ������
	// Flags����־λ���̶�ΪD3D12_FENCE_FLAG_NONE��
	// riid��Ψһ��ʶΧ���ӿ����͵�GUID���̶�ΪIID_ID3D12Fence��
	// ppFence�����ص�ָ��Χ���ӿڵ�ָ���ָ�롣�ں����ɹ�ִ�к󣬸�ָ�뽫ָ��һ��ID3D12Fence�ӿڵ�ʵ����
	ANALYSIS_RESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �����豸���������ǾͿ��Դ������ǵ����������
	// ��ʼ���������

	// �����������
	// D3D12_COMMAND_QUEUE_DESC��DirectX 12�е�һ���ṹ�����ͣ��������������������ʱ�����ԺͲ��������������¼�����Ա������
	// Type��������е����ͣ�������D3D12_COMMAND_LIST_TYPE_DIRECT��D3D12_COMMAND_LIST_TYPE_BUNDLE��D3D12_COMMAND_LIST_TYPE_COMPUTE��D3D12_COMMAND_LIST_TYPE_COPY֮һ��
	// Priority��������е����ȼ���������D3D12_COMMAND_QUEUE_PRIORITY_NORMAL��D3D12_COMMAND_QUEUE_PRIORITY_HIGH֮һ��
	// Flags��������еı�־λ��������D3D12_COMMAND_QUEUE_FLAG_NONE��D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT��D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_IDLE_DEPENDENCY֮һ��
	// NodeMask��CPU�ڵ����룬��ʾ��������п�������ЩCPU�ڵ��ϵ��߳�ʹ�á������Ҫ�ڶ��CPU�ڵ��ϲ���ִ�У�����Ҫʹ�ö��������С�
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};		// ��������
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;				// ָ�������б������(ֱ�����ֱ�ӿ�����GPU��ִ�е�����)
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;				// ָ��������еı�־������ʱ��
	ANALYSIS_RESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	// ����������
	ANALYSIS_RESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&CommandAllocator)
	));

	// ���������б�
	ANALYSIS_RESULT(D3dDevice->CreateCommandList(
		0,		// Ĭ��һ��GPU
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,	// ֱ��GPUִ������
		CommandAllocator.Get(),		// ����ǰ�����б��������������
		nullptr,	// ��Ҫ���뵱ǰ����״̬
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())
	));

	// �����������б���Ҫ���ǽ���ر�
	GraphicsCommandList->Close();

}


