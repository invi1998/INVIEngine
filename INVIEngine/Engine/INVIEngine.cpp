#include "EngineMinimal.h"
#include "EngineFactory.h"

#include "Log/SimpleLog.h"


int Init(CEngine* Engine, HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCMD)
{
#if defined(_WIN32)
	FWinMainCommandParameters WinMainParameters(hInstance, prevInstance, cmdLine, showCMD);
#endif

	int ReturnValue = Engine->PreInit(
#if defined(_WIN32)
		WinMainParameters
#endif
	);
	if (ReturnValue != 0)
	{
		ENGINE_LOG_ERROR("[%i]Engine pre initialization error, check and initialization problem.", ReturnValue)
			return ReturnValue;
	}

	ReturnValue = Engine->Init(
#if defined(_WIN32)
		WinMainParameters
#endif
	);
	if (ReturnValue != 0)
	{
		ENGINE_LOG_ERROR("[%i]Engine initialization error, please check the initialization problem.", ReturnValue)
			return ReturnValue;
	}

	ReturnValue = Engine->PostInit();
	if (ReturnValue != 0)
	{
		ENGINE_LOG_ERROR("[%i]Engine post initialization error, please check initialization problem.", ReturnValue)
			return ReturnValue;
	}

	return ReturnValue;
}

void Tick(CEngine* Engine)
{
	float DeltaTime = 0.03f;
	Engine->Tick(DeltaTime);
	// Sleep(30);
}

int Exit(CEngine* Engine)
{
	int ReturnValue = Engine->PreExit();
	if (ReturnValue != 0)
	{
		ENGINE_LOG_ERROR("[%i]Engine pre exit failed.", ReturnValue)
			return ReturnValue;
	}

	ReturnValue = Engine->Exit();
	if (ReturnValue != 0)
	{
		ENGINE_LOG_ERROR("[%i]Engine exit failed.", ReturnValue)
			return ReturnValue;
	}

	ReturnValue = Engine->PostExit();
	if (ReturnValue != 0)
	{
		ENGINE_LOG_ERROR("[%i]Engine post exit failed.", ReturnValue)
			return ReturnValue;
	}
}


CEngine* Engine = nullptr;


/**
 * \brief 
 * \param hInstance 当前窗口实例
 * \param prevInstance 上次启动进程的对象实例（上次启动的窗口实例）
 * \param cmdLine 传递命令
 * \param showCMD 当前窗口以什么方式显示
 *SW_HIDE	隐藏窗口并激活另外一个窗口
 *SW_MINIMIZE	最小化指定窗口，并激活在系统表顶层的窗口
 *SW_RESTORE 激活并显示窗口，如果窗口已经最小化或者最大化，系统将恢复到原来尺寸的位置显示窗口，（与SW_SHOWNORMAL相同）
 *SW_SHOW 激活一个窗口并以原来的尺寸和位置显示窗口
 *SW_SHOWMAXIMIZED 激活窗口并且将其最大化
 *SW_SHOWMINIMIZED 激活窗口并且将其最小化
 *SW_SHOWMINNOACTIVE 将一个窗口显示为图标，并激活窗口维持活动状态
 *SW_SHOWNA 以窗口的当前状态显示窗口，激活窗口保持活动状态
 *SW_SHOWNOACTIVETE 以窗口的最近一次的尺寸和位置显示窗口，不激活窗口
 *SW_SHOWNORMAL	激活并且显示窗口
 * \return 
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCMD)
{

	int ReturnValue = 0;

	Engine = FEngineFactory::CreateEngine();

	if (Engine)
	{
		Init(Engine, hInstance, prevInstance, cmdLine, showCMD);

		MSG EngineMSG = { nullptr };
		
		while(EngineMSG.message != WM_QUIT)
		{
			// 检索和过滤Windows信息
			// 该行代码的执行效果就是，移除所有的消息，不进行任何过滤
			// PM_NOREMOVE		消息不从消息队列里移除
			// PM_REMOVE		消息从消息队列里移除
			// PM_NOYIELD		此标志使得系统不释放等待调用程序空闲的线程
			//
			// PM_QS_INPUT		处理鼠标和键盘消息
			// PM_QS_PAINT		处理画图信息
			// PM_QS_POSTMESSAGE	处理所有被寄送的消息，包括计时器和热键
			// PM_QS_SENDMESSAGE	处理所有发送消息
			if (PeekMessage(&EngineMSG, 0, 0, 0, PM_REMOVE))
			{
				// 提取完消息后，我们需要对消息进行翻译，翻译为字符串
				TranslateMessage(&EngineMSG);
				// 调配信息，将信息发送给我们的程序窗口，然后我们的程序就能进行使用了 (分发消息）
				DispatchMessage(&EngineMSG);
			}
			else
			{
				Tick(Engine);
			}
		}

		Exit(Engine);

		return 1;
	}
	else
	{
		ReturnValue = 1;
	}


	ENGINE_LOG("[%i]The engine has exited.", ReturnValue)
	return 1;
}
