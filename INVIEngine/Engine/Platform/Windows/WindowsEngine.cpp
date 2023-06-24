#include "WindowsEngine.h"

#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// ��ʼ����־ϵͳ
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	ENGINE_LOG("��־��ʼ�����")

	// ��������


	// �����ӿ�
	if (InitWindows(InParameters))
	{
		
	}

	ENGINE_LOG("����Ԥ��ʼ�����")

	return 0;
}

int FWindowsEngine::Init()
{
	ENGINE_LOG("�����ʼ�����")
	return 0;
}

int FWindowsEngine::PostInit()
{
	ENGINE_LOG("����Post��ʼ�����")
	return 0;
}

void FWindowsEngine::Tick()
{
	
}

int FWindowsEngine::PreExit()
{
	ENGINE_LOG("����Ԥ�˳����")
	return 0;
}

int FWindowsEngine::Exit()
{
	ENGINE_LOG("�����˳����")
	return 0;
}

int FWindowsEngine::PostExit()
{
	ENGINE_LOG("�����˳�post���")
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
		ENGINE_LOG_ERROR("����ע��ʧ��")
		MessageBox(NULL, L"windows class ע��ʧ��,", L"Error", MB_OK);
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
		ENGINE_LOG_ERROR("���ڴ���ʧ��")
		MessageBox(0, L"���ڴ���ʧ��", 0, 0);
		return false;
	}

	// ��ʾ����
	ShowWindow(hwnd, SW_SHOW);

	// ˢ�´���
	UpdateWindow(hwnd);

	ENGINE_LOG("���ڳ�ʼ�����")
}


