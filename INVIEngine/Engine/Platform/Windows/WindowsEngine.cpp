#include "WindowsEngine.h"

#include "WindowsMessageProcessing.h"
#include "Config/EngineRenderConfig.h"
#include "Core/World.h"
#include "Debug/EngineDebug.h"
#include "Mesh/BoxMesh.h"
#include "Mesh/CCylinderMesh.h"
#include "Mesh/ConeMesh.h"
#include "Mesh/CPlaneMesh.h"
#include "Mesh/SphereMesh.h"
#include "Rendering/Core/Rendering.h"

CWindowsEngine::CWindowsEngine()
	: CurrentFenceIndex(0),
	CurrentSwapBufferIndex(0),
	M4XNumQualityLevels(0),
	bMSAA4XEnabled(false),
	BackBufferFormat(DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM), // 纹理格式 默认设置为 8位无符号归一化RGBA格式。（0-255的rgba值 映射到 0-1）
	DepthStencilFormat(DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
{
	bTick = false;

	for (UINT i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}
	
}

int CWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// 处理日志
	constexpr char LogPath[] = "../log";
	init_log_system(LogPath);
	ENGINE_LOG("Log init.");

	// 处理命令


	ENGINE_LOG("引擎预初始化完成");

	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{
	// 处理视口
	InitWindows(InParameters);

	InitDirect3D();

	PostInitDirect3D();

	CWorld* world = CreateObject<CWorld>(new CWorld());

	ENGINE_LOG("引擎初始化完成");

	return 0;
}

int CWindowsEngine::PostInit()
{

	// 初始化命令列表
	ANALYSIS_RESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), nullptr));

	{
		// 构建Mesh
		// CBoxMesh* BoxMesh = CBoxMesh::CreateMesh(1.3f, 2.4f, 3.1f);

		// CSphereMesh* SphereMesh = new CSphereMesh();

		// CCylinderMesh* BoxMesh = CCylinderMesh::CreateMesh(1.0f, 1.0f, 3.0f, 40, 20);

		// CConeMesh* ConeMesh = CConeMesh::CreateMesh(2.5f, 4.0f, 20, 10);

		CPlaneMesh* PlaneMesh = CPlaneMesh::CreateMesh(4.0f, 4.0f, 20, 20);

		for (auto &temp : GObjects)
		{
			temp->BeginInit();
		}
	}

	ANALYSIS_RESULT(GraphicsCommandList->Close());

	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	WaitGPUCommandQueueComplete();
	
	ENGINE_LOG("引擎post初始化完毕");

	return 0;
}

void CWindowsEngine::Tick(float DeltaTime)
{
	for (auto& temp : GObjects)
	{
		if (temp->IsTick())
		{
			temp->Tick(DeltaTime);
		}
	}

	// 重新录制相关内存，为下一帧绘制做准备
	ANALYSIS_RESULT(CommandAllocator->Reset());
	// 预渲染
	for (auto& Render : IRenderingInterface::RenderingInterface)
	{
		Render->PreDraw(DeltaTime);
	}

	// 指向哪个资源，转换器状态，因为我们有两个buffer，他两在不断交换
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuffer(),				// 当前buffer缓冲区
		D3D12_RESOURCE_STATE_PRESENT,		// 当前状态 表示资源即将被用作呈现目标
		D3D12_RESOURCE_STATE_RENDER_TARGET	// 目标状态 这将使资源可以用作渲染目标，并允许您对该资源执行呈现操作（写入状态）
	);
	GraphicsCommandList->ResourceBarrier(
		1,
		&ResourceBarrierPresent
	);

	// 重置（更新）视口信息，裁剪矩阵信息
	GraphicsCommandList->RSSetViewports(1, &ViewPortInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewPortRect);


	// 清除画布
	constexpr float ColorBG[] = { 0.1f, 0.105f, 0.11f, 1.0f };
	GraphicsCommandList->ClearRenderTargetView(
		GetCurrentSwapBufferView(),		// 要清除的渲染目标视图
		ColorBG,		// 画布颜色
		0,		// 后面这两个参数是和视口相关的，这里不在这里设置，后面会有专门的设置方法
		nullptr
	);

	// 清除深度和模板缓冲区
	GraphicsCommandList->ClearDepthStencilView(
		GetCurrentDepthStencilView(),	// 传入CPU内存（要清除的深度模板缓冲区内存）
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,		// 清除深度和模板缓冲区
		1.f,	// 用1来清除我们的深度缓冲区（将深度缓冲区设置为1
		0,		// 用0来清除我们的模板缓冲区（模板缓冲区设置为0
		0,
		nullptr
	);

	// 指定渲染缓冲区（输出合并阶段）设置渲染目标视图
	D3D12_CPU_DESCRIPTOR_HANDLE SwapBufferView = GetCurrentSwapBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView = GetCurrentDepthStencilView();
	GraphicsCommandList->OMSetRenderTargets(
		1,									// 指定渲染目标数 1
		&SwapBufferView,		// 指定渲染目标
		true,								// true表明我们传入的句柄是一个内存连续的描述符指针
		&DepthStencilView		// 传入深度
	);

	// 渲染
	for (auto& Render : IRenderingInterface::RenderingInterface)
	{
		Render->Draw(DeltaTime);
		Render->PostDraw(DeltaTime);
	}

	// 设置当前交换链buffer状态
	CD3DX12_RESOURCE_BARRIER ResourceBarrierRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	GraphicsCommandList->ResourceBarrier(
		1,
		&ResourceBarrierRenderTarget
	);

	// 自此，缓冲区录入完成（记得关闭命令列表），可以提交命令了
	ANALYSIS_RESULT(GraphicsCommandList->Close());

	// 提交命令
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);


	// 交换Buffer缓冲区
	ANALYSIS_RESULT(SwapChain->Present(0, 0));	// 垂直同步0， flags也是0

	CurrentSwapBufferIndex = !static_cast<bool>(CurrentSwapBufferIndex);	// 因为我们的交换链缓冲区就只有2个，所以索引可以简单的使用这种方法设置 （0,1）

	// CPU等待GPU执行结果
	WaitGPUCommandQueueComplete();

}

int CWindowsEngine::PreExit()
{
	return 0;
}

int CWindowsEngine::Exit()
{
	return 0;
}

int CWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();

	return 0;
}

ID3D12Resource* CWindowsEngine::GetCurrentSwapBuffer() const
{
	return SwapChainBuffer[CurrentSwapBufferIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE CWindowsEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),	// 通过RTV堆栈获取当前CPU描述，
		CurrentSwapBufferIndex,	// 传入当前buffer索引（也就是堆栈内存里面，我们要获取的是哪一个，RTV不是有两个缓冲区内存嘛，前后）
		RTVDescriptorSize		// 传入当前RTV描述的大小
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE CWindowsEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT CWindowsEngine::GetDXGISampleCount() const
{
	return bMSAA4XEnabled ? 4 : 1;
}

UINT CWindowsEngine::GetDXGISampleQuality() const
{
	return bMSAA4XEnabled ? (M4XNumQualityLevels - 1) : 0;
}

void CWindowsEngine::WaitGPUCommandQueueComplete()
{
	CurrentFenceIndex++;

	// 向GPU设置新的隔离点 等待GPU处理完信号 （设置信号）
	ANALYSIS_RESULT(CommandQueue->Signal(Fence.Get(), CurrentFenceIndex));

	if (Fence->GetCompletedValue() < CurrentFenceIndex)
	{
		// 创建或者打开一个事件内核对象，并返回该内核对象的句柄
		// 参数1 SECURITY_ATTRIBUTES
		// 参数2 事件名，null表示匿名事件
		// 参数3
		// CREATE_EVENT_INITIAL_SET		0x00000002	表示我们的对象的初始化是否被触发了，FALSE表示未触发
		// CREATE_EVENT_MANUAL_RESET	0x00000001	表示这个事件对象必须要用ResetEvents重置，你不设置这个标志的话，我们的系统内核就会进行默认重置
		// 参数4 事件对象的访问权限
		HANDLE EventEX = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

		// 我们在上面已经设置好信号了，那么在这里信号触发就会通知这个事件，下面这行代码达到的效果就是
		// 我们在上面设置的GPU信号，在GPU完成后，就会通知到事件EventX，从而让CPU知道GPU的命令执行情况
		// GPU完成后悔通知我们当前的事件句柄EventEX
		ANALYSIS_RESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		// 等待信号事件触发，等待时间为无限等待 （等待GPU）
		// 这里会阻塞主线程，防止CPU无限向GPU提交命令
		WaitForSingleObject(EventEX, INFINITE);

		// 如果事件完成了，会释放掉当前事件，那么我们就会在这里接收到这个事件，线程就会在这里被还原，被重新唤醒，然后我们就将当前事件关闭即可
		CloseHandle(EventEX);
	}
}

bool CWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	// 绑定窗口参数
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX);	// 该对象实际占用的内存
	WindowsClass.cbClsExtra = 0;				// 是否需要额外空间 表示窗口类额外内存大小，即每个窗口创建时将分配给该窗口类的额外内存大小
	WindowsClass.cbWndExtra = 0;				// 是否需要额外内存 表示窗口实例额外内存大小，即为每个窗口实例分配的额外内存大小
	// cbClsExtra是用于定义一个窗口类的额外数据的大小，而cbWndExtra是用于定义一个窗口实例的额外数据的大小。通常，开发者可以使用这些额外的内存来存储一些自定义数据或指针，以便在处理消息和其他操作时进行访问。
	WindowsClass.hbrBackground = nullptr;		// 用于指定窗口背景所使用的画刷句柄，如果没有就是设置GDI擦除
	WindowsClass.hCursor = LoadCursor(nullptr, IDC_ARROW);	// 设置箭头光标
	WindowsClass.hIcon = nullptr;				// (我们应用程序放在磁盘的一个图标）
	WindowsClass.hIconSm = nullptr;				// 应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance;	// 窗口实例
	WindowsClass.lpszClassName = L"INVIEngine";	// 窗口名字
	WindowsClass.lpszMenuName = nullptr;		// 菜单名字
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;	// 如何绘制当前窗口（垂直和水平绘制）
	WindowsClass.lpfnWndProc = EngineWidowProc;		// 消息处理函数

	// 注册窗口
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass);

	if (!RegisterAtom)
	{
		MessageBox(NULL, L"Register fail,", L"Error", MB_OK);
	}

	// 视口，视口风格，没有菜单
	RECT Rect = { 0, 0, FEngineRenderConfig::GetRenderConfig()->ScreenWidth, FEngineRenderConfig::GetRenderConfig()->ScreenHeight };
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	int windowWidth = Rect.right - Rect.left;
	int windowHeight = Rect.bottom - Rect.top;

	MainWindowHandle = CreateWindowEx(
		NULL,						// 窗口额外风格
		L"INVIEngine",				// 窗口名称
		L"INVI Engine",				// 显示到标题栏上的窗口名称
		WS_OVERLAPPEDWINDOW,		// 视口风格
		CW_USEDEFAULT, CW_USEDEFAULT,		// 窗口坐标，设置为CW_USEDEFAULT让窗口自适应大小并且显示在屏幕中心
		windowWidth,				// 窗口宽度
		windowHeight,				// 窗口高度
		nullptr,					// 父窗口句柄
		nullptr,					// 菜单句柄
		InParameters.HInstance,		// 窗口实例
		NULL						// 传递进窗口的额外参数
	);

	if (!MainWindowHandle)
	{
		// 窗口创建失败
		MessageBox(0, L"窗口创建失败", 0, 0);
		return false;
	}

	// 显示窗口
	ShowWindow(MainWindowHandle, SW_SHOW);

	// 刷新窗口
	UpdateWindow(MainWindowHandle);
}

bool CWindowsEngine::InitDirect3D()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 开启DX12调试层 debug
	ComPtr<ID3D12Debug> D3d12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3d12Debug))))
	{
		D3d12Debug->EnableDebugLayer();		// 开启调试层
	}

	// HRESULT
	// S_OK					0x00000000	执行成功
	// E_UNEXPECTED			0x8000FFFF	意外的失败
	// E_NOTIMPL			0x80004001	未实现
	// E_OUTOFMEMORY		0x8007000E	未能分配所需内存
	// E_INVALIDARG			0x80070057	一个或者多个参数无效
	// E_NOINTERFACE		0x80004002	不支持此接口
	// E_POINTER			0x80004003	无效指针
	// E_HANDLE				0x80070006	无效句柄
	// E_ABORT				0x80004004	操作终止
	// E_ACCESSDENIED		0x80070005	一般的访问被拒绝错误

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 创建DGI设备驱动

	// 创建DGI实例
	ANALYSIS_RESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGiFactory)));

	// 创建驱动
	//	IUnknown * pAdapter,          // 程序要使用的适配器(设备)实例指针，如果为nullptr则使用默认适配器
	//	D3D_FEATURE_LEVEL          MinimumFeatureLevel,// 最低支持的特性级别
	//	REFIID                     riid,               // 希望通过此函数创建的设备接口类型 ID
		// D3D_FEATUREI_LEVEL_9_ 1 目标功能级别支持Direct3D 9.1  包含shader model 2.
		// D3D_FEATURE_LEVEL_9_2   目标功能级别支持Direct3D 9.2  包含shader model 2.
		// D3D_FEATURE_LEVEL_9_3   目标功能级别支持Direct3D 9.3  包含shader model 3.
		// D3D_FEATURE_LEVEL_10_0  目标功能级别支持Direct3D 10.0 包含shader model 4.
		// D3D_FEATURE_LEVEL_10_1  目标功能级别支持Direct3D 10.1 包含shader model 4.
		// D3D_FEATURE_LEVEL_11_0  目标功能级别支持Direct3D 11.0 包含shader model 5 .

	//	void** ppvDevice        // 返回的指向设备接口的指针的指针
	/*pAdapter：一个指向IDXGIAdapter1或者IDXGIAdapter4的指针，表示程序希望使用的适配器（设备）实例。如果该参数为nullptr，则Direct3D 12会尝试使用默认的适配器。

		MinimumFeatureLevel：最低支持的特性级别，表示应用程序需要使用的硬件特性水平。该参数的值应该是枚举类型D3D_FEATURE_LEVEL中的一个成员，例如D3D_FEATURE_LEVEL_11_0。

		riid：一个唯一标识设备接口类型的GUID。该参数通常可以设置为IID_ID3D12Device，表示程序想要创建一个ID3D12Device对象。

		ppvDevice：返回的指向设备接口的指针的指针。在函数成功执行后，该指针将指向一个ID3D12Device接口的实例*/
	HRESULT D3dDeviceResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// 失败（硬件适配器创建失败）
		// 硬件适配器创建失败，那么这里就采用软件适配器
		// warp 高级光栅化平台

		// 创建适配器
		ComPtr<IDXGIAdapter> WARPAdapter;
		// 枚举适配器
		ANALYSIS_RESULT(DXGiFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		// 创建驱动（使用软件模拟的适配器来创建驱动）
		ANALYSIS_RESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	// 创建fence(创建围栏）为CPU和GPU同步做准备
	// 各个参数含义如下：
	// InitialValue：围栏初始值，表示围栏状态。当GPU执行命令列表时，会写入一个信号值到围栏中，CPU可以通过检查该值来确定GPU是否已经完成了渲染操作。
	// Flags：标志位，固定为D3D12_FENCE_FLAG_NONE。
	// riid：唯一标识围栏接口类型的GUID，固定为IID_ID3D12Fence。
	// ppFence：返回的指向围栏接口的指针的指针。在函数成功执行后，该指针将指向一个ID3D12Fence接口的实例。
	ANALYSIS_RESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 有了设备驱动后，我们就可以创建我们的命令对象了
	// 初始化命令对象

	// 创建命令队列
	// D3D12_COMMAND_QUEUE_DESC是DirectX 12中的一个结构体类型，用于描述创建命令队列时的属性和参数。它包含以下几个成员变量：
	// Type：命令队列的类型，可以是D3D12_COMMAND_LIST_TYPE_DIRECT、D3D12_COMMAND_LIST_TYPE_BUNDLE、D3D12_COMMAND_LIST_TYPE_COMPUTE或D3D12_COMMAND_LIST_TYPE_COPY之一。
	// Priority：命令队列的优先级，可以是D3D12_COMMAND_QUEUE_PRIORITY_NORMAL或D3D12_COMMAND_QUEUE_PRIORITY_HIGH之一。
	// Flags：命令队列的标志位，可以是D3D12_COMMAND_QUEUE_FLAG_NONE、D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT或D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_IDLE_DEPENDENCY之一。
	// NodeMask：CPU节点掩码，表示该命令队列可以由哪些CPU节点上的线程使用。如果需要在多个CPU节点上并发执行，则需要使用多个命令队列。
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};		// 队列描述
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;				// 指定命令列表的类型(直接命令（直接可以在GPU上执行的命令)
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;				// 指定命令队列的标志（不超时）
	ANALYSIS_RESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	// 创建分配器
	ANALYSIS_RESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf())
	));

	// 创建命令列表
	ANALYSIS_RESULT(D3dDevice->CreateCommandList(
		0,															// 默认一个GPU
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,	// 直接GPU执行类型
		CommandAllocator.Get(),										// 将当前命令列表关联到分配器上
		nullptr,													// 需要传入当前管线状态
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())
	));

	// 创建完命令列表，不要忘记将其关闭
	GraphicsCommandList->Close();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 设置多重采样

	// 查询指定硬件和资源格式支持的多重采样质量水平
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.SampleCount = 4;	// 查询采样率4
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;	// 采样质量级别，设置为默认，不支持任何选项
	QualityLevels.NumQualityLevels = 0;
	QualityLevels.Format = BackBufferFormat;
	// 调用CheckFeatureSupport查询我们目前的设备驱动是否支持上面设置的这些采样参数
	ANALYSIS_RESULT(D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,	// 指定检测对象是，我们当前质量的级别
		&QualityLevels,								// 传入质量对象
		sizeof(QualityLevels)						// 传入质量大小
	));

	// 备份质量级别
	M4XNumQualityLevels = QualityLevels.NumQualityLevels;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 交换链

	// 清除交换链，保证我们创建一个新的交换链
	SwapChain.Reset();

	// 交换链描述对象
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	// buffer描述
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;					// 指定宽
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;					// 指定高
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;	// 指定刷新率（分子）
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;													// 指定刷新率（分母）
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;						// 指定交换连的buff数量（缓冲数量，一般都是双缓冲就够了）
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// 指定扫描线的显示顺序（下场优先模式）
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// 将后台缓冲区(表面或资源)用作渲染目标，在其中绘制渲染结果。这是最常见的使用方式，也是默认值
	// 设置纹理
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;		// 纹理格式
	// 窗口设置
	SwapChainDesc.OutputWindow = MainWindowHandle;		// 指定窗口句柄
	SwapChainDesc.Windowed = true;						// 以窗口模式运行(false是全屏）
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;	// 强制刷新模式，在每次呈现新帧时都清空前缓冲区，不保留其内容。这是最常见的交换方式，也是默认值。
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 允许显示模式切换 (即允许我们自由的切换显示窗口，想全屏就全屏，想窗口就窗口）
	// 采样描述(多重采样设置）
	SwapChainDesc.SampleDesc.Count = GetDXGISampleCount();	// 设置采样描述里的采样数量，先判断多重采样是否开启，如果开启，那么赋值为4（默认就是开启4重采样）否则就是0
	SwapChainDesc.SampleDesc.Quality = GetDXGISampleQuality();		// 设置采样描述的质量级别,投影需要判断是否开启多重采样，如果开启，赋值为我们之前设定的采样质量-1，否则为0

	// 创建交换链
	ANALYSIS_RESULT(DXGiFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 资源描述符

	// RTV
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc; // 定义 描述描述符堆（Descriptor Heap）的属性和配置信息 RTV
	// 配置描述符属性
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;						// 描述符数量，这里设置为2，是因为我们使用双缓冲（前台缓冲区，后台缓冲区，所以有两个渲染视图，所以需要两个RTV， 故这里配置2）
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// 指定描述符类型-渲染目标视图
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// 指定描述符默认用法
	RTVDescriptorHeapDesc.NodeMask = 0;								// 指定描述符堆的节点掩码，用于多个GPU节点之间的通信与同步 (0表示不设置，使用默认GPU节点）
	// 配置好属性就可以创建了
	ANALYSIS_RESULT(D3dDevice->CreateDescriptorHeap(&RTVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));		// 使用设备驱动创建RTV描述符堆


	// DSV
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc; // 描述描述符堆（Descriptor Heap）的属性和配置信息 DSV
	// 配置描述符属性
	DSVDescriptorHeapDesc.NumDescriptors = 1;						// 描述符数量，深度缓冲区只需要一个就够了，深度缓冲信息是可以复用的
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	// 指定描述符类型-深度模板视图
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	// 指定描述符默认用法
	DSVDescriptorHeapDesc.NodeMask = 0;								// 指定描述符堆的节点掩码，用于多个GPU节点之间的通信与同步 (0表示不设置，使用默认GPU节点）
	// 配置好属性就可以创建了
	ANALYSIS_RESULT(D3dDevice->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));		// 使用设备驱动创建DSV描述符堆


	return false;
}

void CWindowsEngine::PostInitDirect3D()
{
	// 因为我们在前面有创建相关的内容，所以在这里执行等待（同步）
	// CPU等待GPU执行结果
	WaitGPUCommandQueueComplete();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 将后台缓冲区绑定到渲染流水线

	// 对我们的渲染目标缓冲进行reset
	for (auto& buffer : SwapChainBuffer)
	{
		buffer.Reset();
	}
	// 深度模板也要进行Reset
	DepthStencilBuffer.Reset();

	// 对我们的交换链设置大小
	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,		// 缓冲区数量
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,		// 屏幕宽度
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,		// 屏幕高度
		BackBufferFormat,											// 纹理格式
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH						// 交换链标记（设置这个标记，我们就能通过ResetTarget来在窗口模式和全屏模式之间切换，当我们切换的时候，我们显示模式的那个窗口的分辨率就会进行自适应匹配）
	);

	// 将我们当前的资源绑定到渲染流水线上

	// 获取当前RTV描述符的大小
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// 在我们创建了描述堆以后，我们是需要一个堆句柄来进行访问
	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle);
		// CreateRenderTargetView 创建渲染视图
		// 参数1：指定渲染缓冲区
		// 参数2：指定后台缓冲区格式，因为我们在创建描述堆的时候就已经指定了格式了，所以这里传入nullptr，表示默认
		// 参数3：传入当前RTV渲染目标视图的句柄

		// 创建好渲染视图后，需要做一个偏移的操作，从我们当前缓冲区偏移到下一个缓冲区（因为我们有两个缓冲区，一个前台缓冲区，一个后台缓冲区，偏移量为一个RTV的大小）
		// HeapHandle.ptr += RTVDescriptorSize;
		// 如果使用了微软提供的d3dx12.h那个接口，那么这里就不用自己计算偏移了
		HeapHandle.Offset(1, RTVDescriptorSize);

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 初始化提交资源

	// 资源描述
	D3D12_RESOURCE_DESC ResourceDesc;
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;		// 指定资源宽度
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;		// 指定资源高度
	ResourceDesc.Alignment = 0;														// 对齐方式
	ResourceDesc.MipLevels = 1;														// 指定mipmap的级别
	ResourceDesc.DepthOrArraySize = 1;												// 指定深度（注意，这里如果使用的是3D那么这个指定的含义就是深度，如果是1D或者2D就是指数组的大小
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;					// 指定资源维度,给这个资源定性，这里定性为2D纹理

	ResourceDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;							// 指定采样数
	ResourceDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XNumQualityLevels - 1) : 0;	// 指定采样质量
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;								// 指定纹理格式（每个像素的存储格式）
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;					// 指定标记，设置为该值表示允许我们深度模板（即告诉DX，我们当前这个资源是属于深度模板缓冲的）
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;								// 指定提交资源的排列方式，这里指定为默认，未知排列方式

	// 清除模式
	D3D12_CLEAR_VALUE ClearValue;
	ClearValue.DepthStencil.Depth = 1.f;			// 指定模板深度清理到多少（这里清理到1）
	ClearValue.DepthStencil.Stencil = 0;			// 指定模板（模板清理到多少，清理到0）
	ClearValue.Format = DepthStencilFormat;			// 指定格式

	CD3DX12_HEAP_PROPERTIES Properties(D3D12_HEAP_TYPE_DEFAULT);	// 指定堆类型，设置为默认提交资源堆

	// 创建提交资源
	D3dDevice->CreateCommittedResource(
		&Properties,										// 指定堆类型，设置为默认提交资源堆
		D3D12_HEAP_FLAG_NONE,								// 指定堆动作，不使用特殊标志，即默认提交资源堆
		&ResourceDesc,										// 指定资源描述
		D3D12_RESOURCE_STATE_COMMON,						// 指定标签，表示提交资源的初始状态，这里设置为常规状态，即可以被任何类型的操作所使用
		&ClearValue,										// 指定优化值(指定要清除的提交资源值）
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf())
	);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 深度模板缓冲区

	// 深度缓冲区视图描述
	D3D12_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	DepthStencilViewDesc.Format = DepthStencilFormat;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;							// 指定纹理的索引（描述符索引，这里我们指定第一个Mipmap）
	DepthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;		// 指定深度模板缓冲区的维度（2D纹理）,指定我们需要如何去访问我们深度模板缓冲中的资源
	DepthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;						// 指定视图设置，这里指定默认视图

	// 实例化深度模板缓冲区
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DepthStencilViewDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///绑定到命令列表

	// 同步当前资源状态
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		DepthStencilBuffer.Get(),									// 指向当前深度模板缓冲数组的指针（指向资源的指针）
		D3D12_RESOURCE_STATE_COMMON,								// 表明资源状态前，是一个通用的资源 （资源的当前状态）
		D3D12_RESOURCE_STATE_DEPTH_WRITE);					// 表明资源状态后 是一个 深度写 的资源 （资源的目标状态）
	GraphicsCommandList->ResourceBarrier(
		1,							// 指定资源提交次数
		&Barrier
	);

	// 同步完资源状态后对资源进行关闭
	GraphicsCommandList->Close();

	// 定义一个指针数组，用于存储命令指针列表，它这里可以存放很多条命令
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };

	// 使用队列提交命令
	// _countof是d3d12x.h里用来计算静态分配数组元素的个数，sizeof是用来计算字节数的
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染视口设置(这些设置会覆盖原先的Windows画布)

	// 描述视口尺寸
	// DirectX的坐标系统和OpenGL是不一样的，DX的坐标系原点位于窗口的中心，OpenGL的坐标原点是在屏幕的左下角
	ViewPortInfo.TopLeftX = 0;
	ViewPortInfo.TopLeftY = 0;
	ViewPortInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewPortInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ViewPortInfo.MinDepth = 0.f;	// 最小深度
	ViewPortInfo.MaxDepth = 1.f;	// 最大深度

	// 描述裁剪矩形
	ViewPortRect.top = 0;
	ViewPortRect.left = 0;
	ViewPortRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewPortRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;

	// CPU等待GPU执行结果
	WaitGPUCommandQueueComplete();
}


